/*//================= Some important notes!!!!!! ===================//
SRAM Memory Layout:
+-------------------+ 0x20000000 (SRAM_START)
|                   |
|                   |
|                   |
+-------------------+ T4_STACK_START
| Scheduler/kernel  |
+-------------------+ T3_STACK_START
|   other tasks     |
+-------------------+ T2_STACK_START
|   Task 3 Stack    |
+-------------------+ T1_STACK_START
|   Task 2 Stack    |
+-------------------+ SCHEDULER_STACK_START
|   Task 1 Stack    |
+-------------------+ SRAM_END (0x20005000 for 20KB RAM)

There are two types of stake pointers: MSP and PSP
-- All of the interrupt/exceptions always MSP
-- In thread mode, we can use both MSP and PSP
-- As we have two type of layer: 
                OS layer.Kernel   :- Scheduler
                Application layer  := Tasks
    So, to make saperation in kernel (the scheduler) and the tasks (application)
    we will configure the scheduler to msp. and the tasks to psp



*/




#include <stdint.h>
#include "../Drivers/stm32f103Driver.h"
#include "scheduler.h"
#include "config.h"

 uint8_t current_task = 0;  //task 1 will run first
 uint32_t global_tick_count = 0;
 TCB_t user_tasks [MAX_TASK];



// Set MSP (Main Stack Pointer) for the scheduler
__attribute__((naked)) static void init_scheduler_stack(uint32_t top_of_stack __attribute__((unused))) {
	//(void)top_of_stack;
			//MSR instruction stands for Move to special register. It is used to write values from 
			//general purpose register into special registers
    __asm volatile("MSR MSP, R0"); // Load R0 value (top_of_stack variable) into MSP
	
		//as this funcion is a naked function, there is no prologue or epilogue for this function. 
		//So you have to give instructions to return to the callee function using BX
    __asm volatile("BX LR");       // Return from function
}


__attribute__((naked)) void switch_sp_to_psp(){
	
	//initialize the psp with task1 stack start 
	//because we are first going to lunch task 1
	__asm volatile ("PUSH {LR}"); // to get back to the rtos_init function, we need to preserve LR. because in the next line it will call another function named get_psp_value. After this, we will pop it back
	__asm volatile ("BL get_psp_value");  //get the value of psp of current task
	__asm volatile ("MSR PSP, R0"); //initialize psp
	__asm volatile ("POP {LR}");  //pop back LR
	
	//Change sp to psp using control register
	__asm volatile ("MOV R0, #0x02"); //
	__asm volatile ("MSR CONTROL, R0");
	__asm volatile ("BX LR");


}

uint32_t get_psp_value(){
	
	return user_tasks[current_task].psp_value;

}


void save_psp_value (uint32_t current_psp_value){

		user_tasks[current_task].psp_value = current_psp_value;

}


void update_to_next_task () {
	//state_handler ();
	int taskState = TASK_BLOCKED_STATE;
	for (int i = 0; i< MAX_TASK; i++){
		current_task ++;
		current_task = current_task % MAX_TASK;
		taskState = user_tasks[current_task].curent_state;
		if (taskState == TASK_READY_STATE){
			break;
		}
	}
	
}

void schedule(void) {
	global_tick_count ++;
	global_tick_count = global_tick_count % 100; 
	uint32_t *pICSR = (uint32_t*) 0xE000ED04;
	*pICSR |= (1<<28);


}


// Fill initial stack frame for each task
static void init_task_stack(void) {
	user_tasks[0].curent_state = TASK_READY_STATE;
	user_tasks[1].curent_state = TASK_READY_STATE;
	user_tasks[2].curent_state = TASK_BLOCKED_STATE;
	user_tasks[3].curent_state = TASK_BLOCKED_STATE;
	user_tasks[4].curent_state = TASK_BLOCKED_STATE;
	user_tasks[5].curent_state = TASK_BLOCKED_STATE;
	user_tasks[6].curent_state = TASK_BLOCKED_STATE;

	
	user_tasks[0].psp_value = TASK1_STACK_START;
	user_tasks[1].psp_value = TASK2_STACK_START;
	user_tasks[2].psp_value = TASK3_STACK_START;
	user_tasks[3].psp_value = TASK4_STACK_START;
	user_tasks[4].psp_value = TASK5_STACK_START;
	user_tasks[5].psp_value = TASK6_STACK_START;
	user_tasks[6].psp_value = TASK7_STACK_START;
	
	
	user_tasks[0].task_handler = task1_handler;
	user_tasks[1].task_handler = task2_handler;
	user_tasks[2].task_handler = task3_handler;
	user_tasks[3].task_handler = task4_handler;
	user_tasks[4].task_handler = task5_handler;
	user_tasks[5].task_handler = task6_handler;
	user_tasks[6].task_handler = task7_handler;
	
    uint32_t *psp;
    
    for (int i = 0; i < MAX_TASK; i++) {
        psp = (uint32_t *)user_tasks[i].psp_value;

        psp--; *psp = DUMMY_XPSR;            // xPSR with Thumb bit set
        psp--; *psp = (uint32_t) user_tasks[i].task_handler;      // PC = Task entry point
        psp--; *psp = 0xFFFFFFFD;            // LR = Return to Thread using PSP

        for (int j = 0; j < 13; j++) {
            psp--; *psp = 0;                 // Clear R12, R3-R0, R4-R11
        }

        user_tasks[i].psp_value = (uint32_t)psp;   // Save initial PSP for task
    }
}

__attribute__((naked))  void PendSV_Handler(){
	
	/* Save the ontext of current task */
	
	
	//1. get current running task's psp value
	
	__asm volatile ("MRS R0, PSP");
	//2. Using that psp value, store SF2 (R4 to R11)
	__asm volatile ("STMDB R0!, {R4-R11} ");
	__asm volatile ("PUSH {LR}");
	//3. Save the current value of PSP
	__asm volatile ( " BL save_psp_value " );
	
	
	/* Retrieve the context of next task */
	//1. Decide next task to run
	__asm volatile (" BL update_to_next_task ");
	
	//2. get its past psp value
	__asm volatile ("BL get_psp_value");
	//3. Using that PSP value retrieve SF2 (R4 to R11)
	__asm volatile (" LDMIA R0!, {R4-R11}");
	

	//3. Update PSP and exit
	__asm volatile ("MSR PSP, R0");
	
	__asm volatile ("POP {LR}");
	__asm volatile ("BX LR");



}





//Scheduler
void SysTick_Handler (void){	
	schedule();

}


void scheduler_init (void){
	init_scheduler_stack(SCHEDULER_STACK_START);  //initiate the scheduler first. It will always use MSP	
    init_task_stack();                       //Initialize all of the tasks with dummy context
	Systick_interrupt(system_freq,KERNEL_TICk_PERIOD_MS);
	
	//now our user application/task will happen. 
	//And user application will run on the psp
	//so we have to swithch the stack pointer to use psp
	switch_sp_to_psp();
	task1_handler();
	
	while (1){
	
	}

}




