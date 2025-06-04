// config.h
#ifndef CONFIG_H
#define CONFIG_H

#define hse_freq 8000000UL  //external crystal frequency
#define system_freq  16000000UL //Desired system frequency

 extern uint8_t current_menu_index; 
 extern uint8_t current_time_date[7];
 extern char time_str[6];
 extern char date_str[11];

 



void oled_show_logo ();
void show_menu ();
void home_Screen (const char *time_str, const char *date_str);

#endif
