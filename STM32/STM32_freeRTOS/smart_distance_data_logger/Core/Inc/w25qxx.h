

#ifndef INC_W25QXX_H_
#define INC_W25QXX_H_

#define CS_PORT    GPIOA
#define CS_PIN     GPIO_PIN_4

#include "main.h"
#include <stdio.h>

void SPI_Write(uint8_t *data, uint16_t size) ;
void SPI_Read(uint8_t *data, uint16_t size) ;
void W25Q_WriteEnable(void);
uint8_t W25Q_ReadStatus(void);
uint8_t W25Q_WaitBusy(void) ;
void W25Q_EraseSector(uint32_t addr) ;
void W25Q_Write(uint32_t addr, uint8_t *data, uint16_t len) ;
void W25Q_Read(uint32_t addr, uint8_t *data, uint16_t len) ;

#endif /* INC_W25QXX_H_ */
