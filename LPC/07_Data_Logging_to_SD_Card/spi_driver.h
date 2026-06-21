

#ifndef __SPI__
#define __SPI__

#define WREN  0x06
#define WRDI  0x04
#define RDSR  0x05
#define WRSR  0x01
#define READ  0x03
#define WRITE 0x02



void SPI0_Init(void);
unsigned char SPI0_Transfer(unsigned char data);
void EEPROM_WriteEnable(void);
void EEPROM_WriteByte(unsigned int addr, unsigned char data);
unsigned char EEPROM_ReadByte(unsigned int addr);


#endif
