#include <lpc214x.h>
#include "spi_driver.h"

#define CS (1 << 7)  // P0.7 for EEPROM CS

/* 25LC256 */
static void delay_ms(unsigned int ms) {
    unsigned int i, j;
    for (i = 0; i < ms; i++)
        for (j = 0; j < 10000; j++);
}

void SPI0_Init(void) {
    PINSEL0 |= 0x00001500; // P0.4=SCK0, P0.5=MISO0, P0.6=MOSI0, P0.7=SSEL0
    IO0DIR |= CS;
    IO0SET = CS;
    S0SPCCR = 8;  // SPI clock = PCLK/8
    S0SPCR = 0x20; // Master mode
}

unsigned char SPI0_Transfer(unsigned char data) {
    S0SPDR = data;
    while (!(S0SPSR & 0x80));
    return S0SPDR;
}

// EEPROM Functions
void EEPROM_WriteEnable(void) {
    IO0CLR = CS;
    SPI0_Transfer(WREN);
    IO0SET = CS;
}

void EEPROM_WriteByte(unsigned int addr, unsigned char data) {
    EEPROM_WriteEnable();
    IO0CLR = CS;
    SPI0_Transfer(WRITE);
    SPI0_Transfer((addr >> 8) & 0xFF);
    SPI0_Transfer(addr & 0xFF);
    SPI0_Transfer(data);
    IO0SET = CS;
    delay_ms(10);
}

unsigned char EEPROM_ReadByte(unsigned int addr) {
    unsigned char val;
    IO0CLR = CS;
    SPI0_Transfer(READ);
    SPI0_Transfer((addr >> 8) & 0xFF);
    SPI0_Transfer(addr & 0xFF);
    val = SPI0_Transfer(0x00);
    IO0SET = CS;
    return val;
}
