#include <LPC214x.h>
#include <stdio.h>
#include "lcd.h"
#include "delay.h"

#define PWM_PERIOD 1000

char buf[20];

void ADC0_Init(void)
{
		
    PINSEL1 &= ~(3 << 24);
		PINSEL1 |=  (1 << 24);   // AD0.1  
    AD0CR =
        (1 << 1) |            
        (4 << 8) |             
        (1 << 21);  
		PCONP |= (1U << 12);
}

unsigned int ADC0_Read(void)
{
    AD0CR &= ~(7 << 24);
    AD0CR |= (1 << 24);        
    while (!(AD0GDR & (1UL << 31)));
    return ((AD0GDR >> 6) & 0x3FF);
}


void PWM_Init(void)
{
    PINSEL0 |= (2 << 14);     
    PWMTCR = 0x02;             
    PWMPR = 59;                
    PWMMR0 = PWM_PERIOD;     
    PWMMR2 = 0;                
    PWMMCR = (1 << 1);        
    PWMLER = (1 << 0) | (1 << 2);
    PWMPCR = (1 << 10);        // Enable PWM2 output
    PWMTCR = (1 << 0) | (1 << 3);
}


void PWM_SetDuty(unsigned int duty)
{
    if(duty > PWM_PERIOD)
        duty = PWM_PERIOD;

    PWMMR2 = duty;
    PWMLER = (1 << 2);
}


int main(void)
{
    unsigned int adc_value;
    unsigned int duty_percent;
    unsigned int duty_counts;

		lcd_init();
    ADC0_Init();
    PWM_Init();
		lcd_cmd(0x80);
    lcd_str("   Motor Speed  ");
    lcd_cmd(0xC0);
		lcd_str("    Control    ");
		delay_ms(1000);
    lcd_cmd(0x01);

    while(1)
    {
        adc_value = ADC0_Read();
        duty_percent = (adc_value * 100) / 1023;
        duty_counts = (adc_value * PWM_PERIOD) / 1023;
        PWM_SetDuty(duty_counts);
        lcd_cmd(0x80);
        lcd_str("Motor Speed");
        lcd_cmd(0xC0);
        sprintf(buf, "Duty:%3d%%", duty_percent);
        lcd_str(buf);
        delay_s(1);
    }
}

