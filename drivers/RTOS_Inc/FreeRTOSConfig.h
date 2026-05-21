/*
 * FreeRTOSConfig.h
 *
 *  Created on: Apr 1, 2026
 *      Author: Rishikesh
 */

#ifndef FREERTOS_CONFIG_H
#define FREERTOS_CONFIG_H

#define configUSE_PREEMPTION            1
//#define configCPU_CLOCK_HZ              8000000   // YOU are using HSE = 8 MHz
#define configCPU_CLOCK_HZ             ( ( unsigned long ) 16000000 )
#define configTICK_RATE_HZ              1000      // 1 ms tick

#define configMAX_PRIORITIES            5
#define configMINIMAL_STACK_SIZE        128
#define configTOTAL_HEAP_SIZE           (10 * 1024)

#define configUSE_16_BIT_TICKS          0

#define configUSE_MUTEXES               1
#define configUSE_COUNTING_SEMAPHORES   1

/* Interrupt priorities (VERY IMPORTANT) */
#define configPRIO_BITS                 4

#define configLIBRARY_LOWEST_INTERRUPT_PRIORITY     15
#define configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY 5

#define configKERNEL_INTERRUPT_PRIORITY \
    ( configLIBRARY_LOWEST_INTERRUPT_PRIORITY << (8 - configPRIO_BITS) )

#define configMAX_SYSCALL_INTERRUPT_PRIORITY \
    ( configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY << (8 - configPRIO_BITS) )

#define configUSE_IDLE_HOOK             0
#define configUSE_TICK_HOOK             0

#define INCLUDE_vTaskDelay              1
#define INCLUDE_vTaskDelayUntil         1
#define INCLUDE_vTaskDelete             1

#define vPortSVCHandler     SVC_Handler
#define xPortPendSVHandler  PendSV_Handler
#define xPortSysTickHandler SysTick_Handler

#endif
