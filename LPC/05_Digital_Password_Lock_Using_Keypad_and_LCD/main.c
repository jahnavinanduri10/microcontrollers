
#include <lpc21xx.h>
#include "lcd.h"
#include "delay.h"
#include "keypad.h"

#define GREEN_LED								 0
#define RED_LED  								 1

#define LCD_CLEAR                0x01
#define LCD_LINE1_ADDR           0x80
#define LCD_LINE2_ADDR           0xC0

const char *str = "Password: ";

const unsigned int user_password = 1234;


int main()
{
	unsigned int user_input=0;
	
	IODIR0 |= (1U << GREEN_LED) | (1U << RED_LED);
	
	initkpm();
	lcd_init();
	
	lcd_str("System init...");
	delay_s(1);
	lcd_cmd(LCD_CLEAR);
	
	IOSET0 = (1U << RED_LED);
	
	while(1)
	{
			
			lcd_str(str);
			user_input = Readnum();
			if (user_input == user_password)
			{
				lcd_cmd(LCD_LINE2_ADDR);
				lcd_str("unlocked");
				IOSET0 = (1U << GREEN_LED);
				IOCLR0 = (1U << RED_LED);
			}
			else
			{
				lcd_cmd(LCD_LINE2_ADDR);
				lcd_str("Incorrect pswd");
				IOSET0 = (1U << RED_LED);
				IOCLR0 = (1U << GREEN_LED);
			}
			delay_s(3);
			lcd_cmd(LCD_CLEAR);
	}
}
