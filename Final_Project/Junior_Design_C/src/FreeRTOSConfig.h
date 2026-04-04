#ifndef FREERTOS_CONFIG_H
#define FREERTOS_CONFIG_H

/* Hardware Specific Settings for RP2350 */
#define configCPU_CLOCK_HZ                    ( 150000000UL )
#define configTICK_RATE_HZ                    ( ( TickType_t ) 1000 )
#define configMAX_PRIORITIES                  ( 5 )
#define configMINIMAL_STACK_SIZE              ( 256 ) 

/* RP2350 Specific Port Settings - REQUIRED FOR CORTEX-M33 */
#define configENABLE_FPU                      1
#define configENABLE_MPU                      0
#define configENABLE_TRUSTZONE                0
#define configRUN_FREERTOS_SECURE_ONLY        1
#define configMAX_API_CALL_INTERRUPT_PRIORITY   16
#define configMAX_SYSCALL_INTERRUPT_PRIORITY    16

/* SMP / Multi-core Settings */
#define configNUMBER_OF_CORES                 2
#define configUSE_PASSIVE_IDLE_HOOK           0
#define configTICK_CORE                       0
#define configRUN_MULTIPLE_PRIORITIES         1
#define configUSE_CORE_AFFINITY               1  
/* portCRITICAL_NESTING_IN_TCB removed - port defines this as 0 */
#define configUSE_TASK_PREEMPTION_DISABLE     0

/* SMP Spinlock configuration */
#define configSMP_SPINLOCK_0                  16
#define configSMP_SPINLOCK_1                  17

/* Memory Management */
#define configSUPPORT_DYNAMIC_ALLOCATION      1
#define configTOTAL_HEAP_SIZE                 ( 64 * 1024 ) 
#define configTIMER_TASK_STACK_DEPTH          ( configMINIMAL_STACK_SIZE * 2 )

/* Task Scheduling */
#define configUSE_PREEMPTION                  1
#define configUSE_TIME_SLICING                1
#define configUSE_IDLE_HOOK                   0
#define configUSE_TICK_HOOK                   0
#define configUSE_MUTEXES                     1
#define configUSE_RECURSIVE_MUTEXES           1
#define configUSE_COUNTING_SEMAPHORES         1

/* Software Timers */
#define configUSE_TIMERS                      1
#define configTIMER_TASK_PRIORITY             ( 3 )
#define configTIMER_QUEUE_LENGTH              ( 10 )

/* API Function Inclusion */
#define INCLUDE_vTaskPrioritySet              1
#define INCLUDE_vTaskDelay                    1
#define INCLUDE_vTaskDelete                   1
#define INCLUDE_vTaskSuspend                  1
#define INCLUDE_xQueueGetMutexHolder          1 
#define INCLUDE_xTimerPendFunctionCall        1

/* Misc */
#define configALLOW_MUTEX_SELF_RECURSION      1
#define configUSE_16_BIT_TICKS                0    
#define configIDLE_SHOULD_YIELD               1

#define configASSERT( x ) if( ( x ) == 0 ) { portDISABLE_INTERRUPTS(); for( ;; ); }

#endif /* FREERTOS_CONFIG_H */