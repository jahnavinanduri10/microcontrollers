#include <LPC214x.h>



/*
	P0.0  ----> TRIG
	P0.1  ----> ECHO
	*/
	
static void delay_us(unsigned int us)
{
    unsigned int i, j;
    for(i = 0; i < us; i++)
    {
        for(j = 0; j < 15; j++);   // Adjust if needed
    }
}

void HC_SR04_Init(void)
{
    IO0DIR |=  (1 << 0);   // P0.0 -> TRIG (Output)
    IO0DIR &= ~(1 << 1);   // P0.1 -> ECHO (Input)
    IO0CLR = (1 << 0);     // TRIG low
}

unsigned int HC_SR04_ReadDistance(void)
{
    unsigned int count = 0;
    /* Generate 10us trigger pulse */
    IO0CLR = (1 << 0);
    delay_us(2);
    IO0SET = (1 << 0);
    delay_us(10);
    IO0CLR = (1 << 0);
    /* Wait for ECHO to go HIGH */
    while(!(IO0PIN & (1 << 1)));

    /* Measure HIGH duration */
    while(IO0PIN & (1 << 1))
    {
        count++;
    }
    return (count / 35);
}

