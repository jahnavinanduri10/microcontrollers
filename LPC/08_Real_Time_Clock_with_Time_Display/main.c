#include <lpc21xx.h>
#include <stdio.h>

#include "lcd.h"
#include "rtc.h"
#include "delay.h"


char str[17];

int main()
{
    unsigned int hr,min,sec;
    lcd_init();
    RTC_Init();
    /* Set initial time */
    RTCSetTime(5,47,0);
    while(1)
    {
        RTC_GetTime(&hr,&min,&sec);
        lcd_cmd(0x80);
        sprintf(str,"TIME %02u:%02u:%02u", hr,min,sec);
        lcd_str(str);
        delay_ms(900);
    }
}

