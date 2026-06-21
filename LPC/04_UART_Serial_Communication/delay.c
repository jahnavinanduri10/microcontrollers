#include <stdint.h>

void delay_us(uint32_t us)
{
	  volatile unsigned int i;
    while(us--)
    {
        for(i = 0; i < 20; i++);  // ~1 µs at 60 MHz
    }
}

void delay_ms(uint32_t ms)
{
    while(ms--)
    {
        delay_us(900);
    }
}

void delay_sec(uint32_t sec)
{
    while(sec--)
    {
        delay_ms(1000);
    }
}

