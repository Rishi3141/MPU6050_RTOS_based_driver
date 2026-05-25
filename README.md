# MPU6050 RTOS-Based Driver using STM32F411 and FreeRTOS

## This project implements an RTOS-based MPU6050 driver on STM32F411 using FreeRTOS. The system periodically acquires accelerometer, gyroscope and temperature data, processes it using RTOS tasks, and triggers actions based on processed sensor values.

## Features

- FreeRTOS-based multitasking
- MPU6050 interfacing over I2C
- Queue-based inter-task communication
- Binary semaphore synchronization
- Periodic sensor acquisition using vTaskDelayUntil()
- Threshold-based action task

## Hardware Used

- STM32F411 Discovery Board
- MPU6050 IMU Sensor


## RTOS Task Architecture

| Task | Priority | Function |
|------|------|------|
| Acquisition Task | 5 | Reads MPU6050 data every 10ms |
| Processing Task | 4 | Processes raw sensor data |
| Calibration Task | 4 | Computes gyro offsets |
| Action Task | 5 | Performs action on threshold event |

## Project Structure

Src/        -> Application tasks and main logic
drivers/    -> I2C, GPIO and MPU6050 drivers
Startup/    -> Startup files and linker scripts


## Build Instructions
- Make sure to have FreeRTOS. Add following directories to your compiler include paths : 
 1) FreeRTOSv202406.04-LTS\FreeRTOS-LTS\FreeRTOS\FreeRTOS-Kernel\portable\GCC\ARM_CM4F
 2) FreeRTOSv202406.04-LTS\FreeRTOS-LTS\FreeRTOS\FreeRTOS-Kernel\include
- Open project in STM32CubeIDE
- Build the project
- Flash onto STM32F411 board



# Large heading
## Medium heading
### Small heading

- bullet point 1
- Bullet point 2


```c
xQueueSend(queue, &data, portMAX_DELAY); // codeblock
```

**BOLD_text**

*italic_text*


## Table 
| Task | Priority |
|------|------|
| Acquire | 5 |
| Process | 4 |