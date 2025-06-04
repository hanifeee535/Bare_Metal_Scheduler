#include <stdint.h>
#include "../Drivers/stm32f103Driver.h"
#include "config.h"
#include "../Drivers/lcd_image.h"
#include "scheduler.h"








uint8_t current_time_date[7];
char time_str[6] ;
char date_str[11];
volatile uint16_t ticks = 0; 
volatile uint8_t should_update_display = 0;











int main (){	


	/*By default, internal rc oscillator (HSI) is activeted and the frequency is 8MHz.
	If you need to use HSE and want to configure the clock for more higher frequency, for example
	16/48/64/72 (max) MHZ, you can use  this function: */

	ConfigureSystemClock_HSE( hse_freq, system_freq);

	/*If you want to use HSI and need higher frequency for example: 16, 48, 64(maz) etc, use this function: */

	//ConfigureSystemClock_HSI(system_freq); 
	
	for (int i = 0; i<=5; i++){
		Config_GPIO(portC, i, input, pp_in);
	}
	
	
	// LCD1602_init ();
	// delay_miliSecond(3, 50,system_freq);
	// LCD1602_print (1, 2, "hello world");

	// Define the struct instance
	
	
	 
	

	oled_configure_64 (I2C_1);
	i2c_init(I2C_2,i2c_FastMode); 
	oled_blank(I2C_1);	
	oled_show_logo ();
	oled_blank(1);
	//show_menu ();
	// update_time(I2C_2, 1, 4, 0);
	// update_date(I2C_2 , 2, 6, 25);

	//read_rtc_value ();
	home_Screen (time_str, date_str);
	
	scheduler_init ();
	// systic_init();

	// 


	while (1){
		// 	if (should_update_display) {
        // 	should_update_display = 0;  // Clear flag
        // 	read_rtc_value ();        	
        // 	home_Screen (time_str, date_str);
    	// }
            
		
		}
	


}