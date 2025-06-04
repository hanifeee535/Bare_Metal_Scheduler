#include <stdint.h>




//Kernel tick period
#define KERNEL_TICk_PERIOD_MS 1


#define TASK_READY_STATE 0x00
#define TASK_BLOCKED_STATE 0xFF

//Number of Tasks
#define MAX_TASK 7

//Dummy XPSR values which will be needed while initialize tasks
#define DUMMY_XPSR 0x01000000U

//Stack memory calculations
#define SIZE_TASK_STACK						    1024U //each task skack is 1KB/1024 bytes
#define SIZE_SCHEDULER_STACK			        1024U //Same for the schedulers

//SRAM calculations
#define SRAM_START 								0x20000000U
#define SRAM_SIZE 								( (20) * (1024) )
#define SRAM_END 								((SRAM_START) + (SRAM_SIZE))

//Stack memory calulations
/*
    Note: ARM cortex MX processor uses full descending stack
    So, the first stack will be started from the end of the SRAM
    In this case, we have total 8 stack: task 1-6 stack, one idle task and onother one 
    is for the scheduler itself

*/
#define TASK1_STACK_START 						(SRAM_END)
#define TASK2_STACK_START						((SRAM_END)- (1*SIZE_TASK_STACK))
#define TASK3_STACK_START					    ((SRAM_END)- (2*SIZE_TASK_STACK))
#define TASK4_STACK_START						((SRAM_END)- (3*SIZE_TASK_STACK))
#define TASK5_STACK_START						((SRAM_END)- (4*SIZE_TASK_STACK))
#define TASK6_STACK_START						((SRAM_END)- (5*SIZE_TASK_STACK))
#define TASK7_STACK_START						((SRAM_END)- (6*SIZE_TASK_STACK))
#define SCHEDULER_STACK_START			        ((SRAM_END)- (7*SIZE_TASK_STACK))


typedef struct {
    uint32_t psp_value;
    uint32_t block_count;
    uint8_t curent_state;
    void (*task_handler)(void);
} TCB_t;


extern uint8_t current_task;
extern uint32_t global_tick_count;
extern TCB_t user_tasks[MAX_TASK];




void task1_handler(void); //IDLE task
void task2_handler(void); 
void task3_handler(void); 
void task4_handler(void); 
void task5_handler(void); 
void task6_handler(void); 
void task7_handler(void);  




//scheduler API
void scheduler_init (void);
void state_handler ();