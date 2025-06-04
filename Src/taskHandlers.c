
#include <stdint.h>
#include "../Drivers/stm32f103Driver.h"
#include "scheduler.h"
#include "config.h"
volatile uint8_t state = 0; 

void read_time_and_date (){
	get_time_and_date(I2C_2, current_time_date);
	format_time_string(current_time_date[2], current_time_date[1], time_str);
	format_date_string(current_time_date[4], current_time_date[5], current_time_date[6], date_str);

}




//IDLE Task
void task1_handler(void){

    while(1){
      //idle task code
        
    };
}

//RTC Value Update and Home screan Task
void task2_handler(void){
    while(1){
       
        read_time_and_date ();
        home_Screen (time_str, date_str);
    };
}


void task3_handler(void){
    while(1){
        //task3 code
    };
}
void task4_handler(void){
    while(1){
       //task4 code
    };
}

void task5_handler(void){
    while(1){
        //task 5 code
    };
}

void task6_handler(void){
    while(1){
        //task 6 code
    };
}

void task7_handler(void){
    while(1){
        //task 7 code
    };
}