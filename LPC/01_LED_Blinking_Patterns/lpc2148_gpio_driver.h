
#ifndef __GPIO_DRIVER_H__
#define __GPIO_DRIVER_H__
#include <stdint.h>
#define PORT0	0
#define PORT1 1

#define HIGH 1
#define LOW 0




/************************
* PUBLIC APIs*
************************/

void pinWrite(uint8_t port,uint8_t pinNumber,uint8_t state);
uint8_t pinRead(uint8_t port, uint8_t pinNumber);
void pinToggle(uint8_t port, uint8_t pinNumber);
void portWrite(uint8_t port,uint32_t value);
uint32_t portRead(uint8_t port);
void pinSelect(int pinNr, int spFunction);


#endif
