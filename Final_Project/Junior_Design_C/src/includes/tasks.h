#ifndef _TASKS_H
#define _TASKS_H

#include <stdio.h>
#include "pico/stdlib.h"
#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"

#define MAX_MAP_SIZE 256

// make a struct for the stored data
typedef struct{
    char genre[16];
    int progress;
} LibraryLog;

// initialize a list of all the data;
extern LibraryLog DataMap[MAX_MAP_SIZE];

void Progress_Task(void *pvParameters);
void button_isr(uint gpio, uint32_t events);
void Button_Task(void *pvParameters);
void Switch_Task(void *pvParameters);
void Chime_Task(void *pvParameters);
void Photo_Task(void *pvParameters);
void Log_Task(void *pvParameters);

extern TaskHandle_t xProgressHandle;
extern TaskHandle_t xButtonHandle;
extern TaskHandle_t xSwitchHandle;

#endif /*_TASKS_H*/