#ifndef _TASKS_H
#define _TASKS_H

#include <stdio.h>
#include "pico/stdlib.h"
#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"

void Heartbeat_Task(void *pvParameters);
void button_isr(uint gpio, uint32_t events);
void Button_Task(void *pvParameters);
void Switch_Task(void *pvParameters);
void Chime_Task(void *pvParameters);

#endif /*_TASKS_H*/