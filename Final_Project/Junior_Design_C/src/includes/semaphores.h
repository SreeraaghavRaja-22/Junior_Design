#ifndef _SEMAPHORES_H
#define _SEMAPHORES_H

#include "FreeRTOS.h"
#include "pico/stdlib.h"
#include "semphr.h"

extern SemaphoreHandle_t xButtonSemaphore;
extern SemaphoreHandle_t xLedMutex;
extern SemaphoreHandle_t xChimeSemaphore;
extern SemaphoreHandle_t xADCMutex;


#endif /*_SEMAPHORES_H */