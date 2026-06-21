
/*  LED Pattern */

#include <lpc21xx.h>
#include "delay.h"
#include "lpc2148_gpio_driver.h"

#define LED1   0
#define LED2   1
#define LED3   2
#define LED4   3

int main(){
	IODIR0 |= 0xF;
	while(1)
	{
		pinWrite(PORT0,LED1,HIGH);
		delay_ms(100);
		pinWrite(PORT0,LED1,LOW);
		delay_ms(100);
		
		pinWrite(PORT0,LED2,HIGH);
		delay_ms(100);
		pinWrite(PORT0,LED2,LOW);
		delay_ms(100);
		
		pinWrite(PORT0,LED3,HIGH);
		delay_ms(100);
		pinWrite(PORT0,LED3,LOW);
		delay_ms(100);
		
		pinWrite(PORT0,LED4,HIGH);
		delay_ms(100);
		pinWrite(PORT0,LED4,LOW);
		delay_ms(100);
	}
}
