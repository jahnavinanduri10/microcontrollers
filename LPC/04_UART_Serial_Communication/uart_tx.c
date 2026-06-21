#include<LPC214x.h>
#include "uart.h"
#include "delay.h"

static char msg[]="Hello LPC21xx\r\n";
int main(){
UART0_Init(9600);
	
	while(1){
		UART0_SendString((uint8_t*)msg);
		delay_ms(500);
	}
}
