
#include <lpc21xx.h>
#include <stdint.h>
void UART0_Init(uint32_t baud)
{
	PINSEL0 |= 0x00000005;
	U0LCR = 0x83;

	if ( baud == 9600){
		U0DLL = 78;
	}
	else if ( baud ==115200) U0DLL = 0x08;
	U0DLM = 0;
	U0LCR = 0x03;
}

void UART1_Init(uint32_t baud)
{
	PINSEL0 |= 0x00050000;
	U1LCR = 0x83;

	if ( baud ==9600){
		U1DLL = 97;
	}
	else if ( baud ==115200) U1DLL = 0x08;
	U1DLM = 0;
	U1LCR = 0x03;
}

void UART0_SendChar(uint8_t ch)
{
		while(!(U0LSR & 0x20));
		U0THR = ch;
}

void UART1_SendChar(uint8_t ch)
{
		while(!(U1LSR & 0x20));
		U1THR = ch;
}


uint8_t UART0_ReceiveChar(void)
{
	while(!(U0LSR & 0x01)){}	// Wait until Receive Data Ready (RDR bit 0 is set)
  return (uint8_t)U0RBR;
}

uint8_t UART1_ReceiveChar(void)
{
	while(!(U1LSR & 0x01)){}     // Wait until Receive Data Ready (RDR bit 0 is set)
  return (uint8_t)U1RBR;
}


void UART0_SendString(uint8_t* str)
{
	uint16_t i;
	for(i=0;str[i]!='\0';i++)
		UART0_SendChar(str[i]);
}
void UART1_SendString(uint8_t *str)
{
	uint16_t i;
	for(i=0; str[i];i++)
	{
		UART1_SendChar(str[i]);
	}
}

void UART0_ReceiveString(uint8_t *buffer, uint32_t bytes)
{
    uint32_t i = 0;
    uint8_t ch;
    while(i < (bytes - 1))        // leave space for '\0'
    {
        ch = UART0_ReceiveChar();  // blocks and waits ?
        if(ch == '\r' || ch == '\n')  // stop on Enter key
            break;
        buffer[i++] = ch;
    }
    buffer[i] = '\0';             // null terminate ?
}

void UART1_ReceiveString(uint8_t *buffer, uint32_t bytes)
{
    uint32_t i = 0;
    uint8_t ch;
    while(i < (bytes - 1))        // leave space for '\0'
    {
        ch = UART1_ReceiveChar();  // blocks and waits ?
        if(ch == '\r' || ch == '\n')  // stop on Enter key
            break;
        buffer[i++] = ch;
    }
    buffer[i] = '\0';             // null terminate ?
}
