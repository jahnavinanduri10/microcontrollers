
#include <lpc21xx.h>
#include <stdint.h>
#include "lpc2148_gpio_driver.h"

void pinWrite(uint8_t port,uint8_t pinNumber,uint8_t state)
{
	if(port == PORT0)
	{
		if ( state == HIGH )
			IOSET0 |= ( 1 << pinNumber );
		else if ( state == LOW )
			IOCLR0 |= ( 1 << pinNumber );
	}
	else if( port == PORT1)
	{
		if ( state == HIGH )
			IOSET1 |= ( 1 << ( pinNumber + 16 ));
		else if ( state == LOW )
			IOCLR1 |= ( 1 << ( pinNumber + 16 ));
	}
	return;
}

uint8_t pinRead(uint8_t port, uint8_t pinNumber)
{
	if(port == PORT0)
	{
		return ( IOPIN0 >> pinNumber )&0x01;
	}
	else if( port == PORT1)
	{
		return ( IOPIN1 >> (pinNumber +16));
	}
	return LOW;
}
void pinToggle(uint8_t port, uint8_t pinNumber)
{
	if(port == PORT0)
	{
		IOPIN0 ^= ( 1 << pinNumber );
	}
	else if( port == PORT1)
	{
		IOPIN1 ^= ( 1 << (pinNumber +16));
	}
	return;
}

void portWrite(uint8_t port,uint32_t value)
{
	if(port == PORT0)
	{
		IOPIN0 = value;
	}
	else if( port == PORT1)
	{
		IOPIN1 = (value << 16);
	}
	return;
}

uint32_t portRead(uint8_t port)
{
	uint32_t temp=0;
	if( port == PORT0)
		temp= IOPIN0 & 0x00FFFFFF;
	else if(port == PORT1)
		temp = IOPIN1 >> 16;
	return temp;
}


void pinSelect(int pinNr, int spFunction) {
    int bit0, bit1;

    if (spFunction == 0)      { bit1 = 0; bit0 = 0; }
    else if (spFunction == 1) { bit1 = 0; bit0 = 1; }
    else if (spFunction == 2) { bit1 = 1; bit0 = 0; }
    else if (spFunction == 3) { bit1 = 1; bit0 = 1; }
    else return;

    if (pinNr >= 0 && pinNr <= 15) {
        if (bit0) PINSEL0 |=  (1 << (pinNr * 2));
        else      PINSEL0 &= ~(1 << (pinNr * 2));

        if (bit1) PINSEL0 |=  (1 << ((pinNr * 2) + 1));
        else      PINSEL0 &= ~(1 << ((pinNr * 2) + 1));
    }
    else if (pinNr >= 16 && pinNr <= 31) {
        pinNr -= 16;
        if (bit0) PINSEL1 |=  (1 << (pinNr * 2));
        else      PINSEL1 &= ~(1 << (pinNr * 2));

        if (bit1) PINSEL1 |=  (1 << ((pinNr * 2) + 1));
        else      PINSEL1 &= ~(1 << ((pinNr * 2) + 1));
    }
    else if (pinNr >= 116 && pinNr <= 131) {
        pinNr -= 116;
        if (bit0) PINSEL2 |=  (1 << (pinNr * 2));
        else      PINSEL2 &= ~(1 << (pinNr * 2));

        if (bit1) PINSEL2 |=  (1 << ((pinNr * 2) + 1));
        else      PINSEL2 &= ~(1 << ((pinNr * 2) + 1));
    }
}

