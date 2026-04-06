#include "FreeRTOS.h"
#include "pico/stdlib.h"
#include "semphr.h"

SemaphoreHandle_t xButtonSemaphore;
SemaphoreHandle_t xLedMutex;
SemaphoreHandle_t xChimeSemaphore;
SemaphoreHandle_t xADCMutex;