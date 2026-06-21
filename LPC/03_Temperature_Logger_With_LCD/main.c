#include <lpc214x.h>
#include <stdio.h>
#include "lcd.h"

void delay_ms(unsigned int ms)
{
    unsigned int i,j;

    for(i=0;i<ms;i++)
        for(j=0;j<6000;j++);
}

void ADC_Init(void)
{
    /* P0.28 -> AD0.1 */
		 
    PINSEL1 &= ~(3 << 24);
    PINSEL1 |=  (1 << 24);

    AD0CR = (1 << 1)      |   // Select AD0.1
            (13 << 8)      |   // CLKDIV
            (1 << 21);        // ADC Enable
	PCONP |= (1 << 12);
}

unsigned int ADC_Read(void)
{
    AD0CR &= ~(7 << 24);      // Clear START bits
    AD0CR |=  (1 << 24);      // Start conversion

    while(!(AD0GDR & (1UL << 31)));

    return ((AD0GDR >> 6) & 0x3FF);
}

float LM35_ReadTemp(void)
{
    unsigned int adc;
    float voltage;
		char msg[20];
	adc = ADC_Read();
		sprintf(msg,"%u",adc);
		lcd_cmd(0x01);
		lcd_str(msg);
	while(1);
    voltage = ((float)adc * 3.3f) / 1023.0f;
    return voltage * 100.0f;
}

int main(void)
{
    float temperature;
    char msg[17];

    lcd_init();

    ADC_Init();
	lcd_str("Hello");
	delay_ms(1000);
	lcd_cmd(0x01);
	
    while(1)
    {
        temperature = LM35_ReadTemp();
        lcd_cmd(0x80);               // Row 1 Col 1
        sprintf(msg,"Temp:%d C ",(int)temperature);
        lcd_str(msg);
        delay_ms(500);
    }
}




