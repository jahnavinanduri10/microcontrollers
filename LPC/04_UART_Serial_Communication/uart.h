
#ifndef __UART__
#define __UART__
#include <stdint.h>

void UART0_Init(uint32_t);
void UART1_Init(uint32_t);

void UART0_SendChar(uint8_t ch);
void UART1_SendChar(uint8_t ch);

uint8_t UART0_ReceiveChar(void);
uint8_t UART1_ReceiveChar(void);

void UART0_SendString(uint8_t *);
void UART1_SendString(uint8_t *);

void UART0_ReceiveString(uint8_t *,uint32_t);
void UART1_ReceiveString(uint8_t *,uint32_t);

#endif
