#include <stdio.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "../main_init.h"
#include "../semaphores.h"
#include "../lcd_init.h"
#include "math.h"
#include "../chime.h"
#include "../tasks.h"

TaskHandle_t xProgressHandle  = NULL;
TaskHandle_t xButtonHandle    = NULL;
TaskHandle_t xSwitchHandle    = NULL;

// ----------------------- State Flags ----------------------- //
volatile bool start_flag       = 0;  // when program starts
volatile bool progress_flag    = 0;  // for the progress bar
volatile bool genre_flag       = 0;  // for genre chooing
volatile bool book_rec_flag    = 0;  // dark mode: book recommendation
volatile int  log_inx          = 0;  // index to track current location in datamap
volatile bool log_mode         = false; // suppress chime while browsing logs

// ----------------------- Photo State ----------------------- //
typedef enum { PHOTO_NORMAL, PHOTO_DARK, PHOTO_BRIGHT } photo_mode_t;
volatile photo_mode_t photo_mode    = PHOTO_NORMAL;
volatile bool         photo_enabled = true;

// ----------------------- Log into MAP ----------------------- //
char current_genre[16] = "Mystery";
int current_progress   = 0;
LibraryLog DataMap[MAX_MAP_SIZE];

// ----------------------- LUTs ----------------------- //

// Shared genre names — used by normal mode and dark (book rec) mode
static const char *genres[16] = {
    "Mystery",      "Romance",       "Horror",      "Fantasy",
    "Psychological","Sci-Fi",        "Classics",    "Historical",
    "Comedy",       "Philosophy",    "Classics",    "Biography",
    "Adventure",    "Manga/Comics",  "True Crime",  "LGBTQ+",
};

static const char *book_recs[16] = {
    "Gone Girl",        // Mystery
    "Pride&Prejudice",  // Romance
    "Pet Sematary",     // Horror
    "Mistborn",         // Fantasy
    "Fight Club",       // Psychological
    "Dune",             // Sci-Fi
    "1984",             // Classics
    "All Quiet...",     // Historical
    "Good Omens",       // Comedy
    "Fear&Trembling",   // Philosophy
    "Brave New World",  // Classics
    "Night",            // Biography
    "Into the Wild",    // Adventure
    "Frieren",          // Manga/Comics
    "In Cold Blood",     // True Crime
    "Heated Rivalry",  // LGBTQ+
};

// 15 new genres for bright mode 
static const char *bright_genres[16] = {
    "Satire",
    "Thriller",
    "Self-Help",
    "Poetry",
    "Drama",
    "Western",
    "Dystopian",
    "Mythology",
    "Military",
    "Political",
    "Travel",
    "Art & Design",
    "Music",
    "Sports",
    "Cooking",
    "Nature",
};

// ----------------------- Tasks ----------------------- //
void Progress_Task(void *pvParameters) {
    bool rst = 0;
    int prev_leds_on = 0;
    char buf[16];
    for(;;) {
        if(progress_flag){
            xSemaphoreTake(xADCMutex, portMAX_DELAY);
            adc_select_input(POT_ADC);
            uint16_t raw = adc_read();
            xSemaphoreGive(xADCMutex);

            printf("Raw Value: 0x%03x, voltage = %f V\n", raw, raw * ADC_CONV_FACTOR);
            float voltage_adc = raw * ADC_CONV_FACTOR;
            float threshold = voltage_adc / 3.3f;
            int leds_on = (int)roundf(threshold * 5);

            xSemaphoreTake(xLedMutex, portMAX_DELAY);
            for(int i = LED1_PIN; i < LED1_PIN + 5; i++){
                if((i - LED1_PIN) < leds_on){ gpio_put(i, 1); }
                else{ gpio_put(i, 0); }
            }
            xSemaphoreGive(xLedMutex);

            if(abs(prev_leds_on - leds_on) >= 1){
                int percent = leds_on * 20;
                snprintf(buf, sizeof(buf), "Progress: %d%%", percent);
                current_progress = percent;
                lcd_clear();
                lcd_print(buf);
            }
            prev_leds_on = leds_on;
            rst = 1;
        }
        else{
            if(rst){
                lcd_clear();
                lcd_print("Registration");
                lcd_set_cursor(1, 0);
                lcd_print("Complete!");
                rst = 0;
            }
        }
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}

void button_isr(uint gpio, uint32_t events){
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    xSemaphoreGiveFromISR(xButtonSemaphore, &xHigherPriorityTaskWoken);
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

void Button_Task(void *pvParameters){
    for(;;){
        if(xSemaphoreTake(xButtonSemaphore, portMAX_DELAY) == pdTRUE){
            vTaskDelay(pdMS_TO_TICKS(100)); // debounce
            if(!gpio_get(BUTTON_PIN) && !log_mode){
                if(!start_flag){
                    // --- Entry: choose path based on light condition ---
                    start_flag = 1;
                    photo_enabled = false;
                    if(photo_mode == PHOTO_DARK){
                        book_rec_flag = 1;
                    }
                    // PHOTO_BRIGHT and PHOTO_NORMAL both use the normal progress/genre flow
                    // photo_mode stays frozen (photo_enabled=false) so Switch_Task can check it
                }
                else{
                    // --- Exit / advance: depends on active state ---
                    if(book_rec_flag){
                        // one button press exits book rec state
                        book_rec_flag = 0;
                        start_flag    = 0;
                        photo_enabled = true;
                    }
                    else{
                        // Normal mode state machine: genre → progress → done
                        if(!genre_flag && !progress_flag){
                            genre_flag = 1;
                        }
                        else if(genre_flag && !progress_flag){
                            genre_flag    = 0;
                            progress_flag = 1;
                        }
                        else{
                            progress_flag = 0;
                            start_flag    = 0;
                            photo_enabled = true;
                            DataMap[log_inx].progress = current_progress;
                            snprintf(DataMap[log_inx].genre, 16, "%s", current_genre);
                            log_inx = (log_inx + 1) % MAX_MAP_SIZE;
                            xSemaphoreGive(xChimeSemaphore);
                        }
                    }
                }
            }

            xSemaphoreTake(xLedMutex, portMAX_DELAY);
            for(int i = LED1_PIN; i < LED1_PIN + 5; i++) gpio_put(i, 0);
            xSemaphoreGive(xLedMutex);

            while(xSemaphoreTake(xButtonSemaphore, 0) == pdTRUE);
        }
    }
}

void Switch_Task(void *pvParameters){
    uint8_t last_state = 0xFF; // used to only refresh LCD when necessary
    uint8_t log_switch;

    // different genres
    typedef enum { SW_BOOK_REC, SW_GENRE, SW_IDLE } sw_mode_t;
    sw_mode_t prev_mode = SW_IDLE;
    bool use_bright = false; 
    const char* genre;

    for(;;){
        uint8_t switches = 0;
        switches |= gpio_get(SWITCH1_PIN) << 0;
        switches |= gpio_get(SWITCH2_PIN) << 1;
        switches |= gpio_get(SWITCH3_PIN) << 2;
        switches |= gpio_get(SWITCH4_PIN) << 3;

        // Determine current mode
        sw_mode_t cur_mode;
        if(book_rec_flag) cur_mode = SW_BOOK_REC;
        else if(genre_flag) cur_mode = SW_GENRE;
        else                cur_mode = SW_IDLE;

        // Reset last_state on mode transition so display refreshes immediately
        if(cur_mode != prev_mode){
            last_state = 0xFF;
            if(prev_mode == SW_BOOK_REC){
                lcd_clear();
                lcd_print("Luminous Library");
            }
            if(cur_mode == SW_GENRE){
                // Snapshot photo_mode once so bright vs normal stays stable
                use_bright = (photo_mode == PHOTO_BRIGHT);
            }
            prev_mode = cur_mode;
        }

        if(cur_mode == SW_BOOK_REC){
            if(switches != last_state){
                last_state = switches;
                lcd_clear();
                lcd_print(genres[switches]);
                lcd_set_cursor(1, 0);
                lcd_print(book_recs[switches]);
            }
        }
        else if(cur_mode == SW_GENRE){
            if(switches != last_state){
                last_state = switches;
                lcd_clear();
                genre = use_bright ? bright_genres[switches] : genres[switches];
                lcd_print(genre);
                snprintf(current_genre, 16, "%s", genre);
            }
        }
        else{ // SW_IDLE
        }

        vTaskDelay(pdMS_TO_TICKS(50));
    }

}

void Chime_Task(void *pvParameters){
    for(;;){
        if(xSemaphoreTake(xChimeSemaphore, portMAX_DELAY) == pdTRUE){
            if(!log_mode){
                chime_start(CHIME_SAMPLE_RATE);
                while(!start_flag && !log_mode){
                    vTaskDelay(pdMS_TO_TICKS(100));
                }
                chime_stop();
            }
        }
    }
}

void Photo_Task(void *pvParameters){
    for(;;){
        if(photo_enabled){
            xSemaphoreTake(xADCMutex, portMAX_DELAY);
            adc_select_input(PHO_ADC);
            uint16_t raw = adc_read();
            xSemaphoreGive(xADCMutex);

            float voltage = raw * ADC_CONV_FACTOR;
            printf("Photo: 0x%03x, %.3f V\n", raw, voltage);

            if(voltage >= 2.0f){
                photo_mode = PHOTO_DARK;
            } else if(voltage < 0.5f){
                photo_mode = PHOTO_BRIGHT;
            } else {
                photo_mode = PHOTO_NORMAL;
            }
        }
        vTaskDelay(pdMS_TO_TICKS(200));
    }
}


void Log_Task(void *pvParameters) {
    for (;;) {
        // Wait until GP6 is high
        if (gpio_get(LOG_SWITCH)) {
            vTaskDelay(pdMS_TO_TICKS(50)); // Debounce
            if (gpio_get(LOG_SWITCH)) {

                log_mode = true;
                chime_stop();

                printf("GET FRICKED!");

                if (log_inx == 0) {
                    lcd_clear();
                    lcd_print("No Logs Found");
                    vTaskDelay(pdMS_TO_TICKS(1500));
                } else {
                    int i = 0;
                    while (i < log_inx && gpio_get(LOG_SWITCH)) {
                        lcd_clear();
                        lcd_print(DataMap[i].genre);

                        lcd_set_cursor(1, 0);
                        char buf[16];
                        snprintf(buf, sizeof(buf), "Prog: %d%%", DataMap[i].progress);
                        lcd_print(buf);

                        i++;
                        vTaskDelay(pdMS_TO_TICKS(1500));
                    }
                }

                // Wait for switch release before restoring display
                while(gpio_get(LOG_SWITCH)){
                    vTaskDelay(pdMS_TO_TICKS(50));
                }
                log_mode = false;
                lcd_clear();
                lcd_print("Luminous Library");

            }
        }
        // Check every 100ms so we don't waste CPU
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}