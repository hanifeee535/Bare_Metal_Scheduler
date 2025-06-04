#  Preemptive Round-Robin Scheduler for ARM Cortex-M3

## Description:
This project implements a **basic preemptive RTOS-style scheduler** from scratch for ARM Cortex-M3 microcontrollers (e.g., STM32F103). It uses the `SysTick` timer to trigger task switches at a regular interval, giving each task a fair share of CPU time.

##  Features

-  **Preemptive round-robin scheduling**
-  **Separate PSP stacks** for each task
-  Up to 7 concurrent tasks
-  Context switching in `SysTick_Handler`
-  Clean stack initialization (xPSR, PC, LR, R0–R11)
-  Idle task support (optional)
-  Written in C with inline ARM assembly

##  Stack Layout

Memory layout separates stacks for each task and scheduler:

SRAM calculations
 SRAM_START 	0x20000000U
 SRAM_SIZE 	( (20) * (1024) )
 SRAM_END 	((SRAM_START) + (SRAM_SIZE))

+-------------------+  (SRAM END)
| Task 1 Stack |
+-------------------+
| Task 2 Stack |
+-------------------+
| Task 3 Stack |
+-------------------+
| Task 4 Stack  |
+-------------------+
| Task 5 Stack  |
+-------------------+
| Task 6 Stack  |
+-------------------+
| Task 7 Stack  |
+-------------------+
| Scheduler Stack |
+-------------------+
| --------------- |
+-------------------+
| -------------- |
+-------------------+ 0x20000000 (SRAM START)



##  Architecture

- **SysTick** triggers context switches.
- **MSP** is used for the kernel/scheduler.
- **PSP** is used for all user/application tasks.
- Registers R4–R11 are saved/restored manually.
- Tasks are set up with dummy stack frames to simulate a hardware exception return.


##  Toolchain

- **Compiler**: `arm-none-eabi-gcc`
- **Debugger**: Keil uVision 5 / OpenOCD / ST-Link
- **MCU**: STM32F103 (20KB SRAM, 72 MHz)

## Acknowledgement:
 Fastbit Embedded Brain Academy


