/*
 * flash.h
 *
 *  Created on: May 22, 2026
 *      Author: SANTOSH
 */

#ifndef INC_FLASH_H_
#define INC_FLASH_H_

void SPI_Write(uint8_t *data, uint16_t size) ;
void SPI_Read(uint8_t *data, uint16_t size) ;
void W25Q_WriteEnable(void);
uint8_t W25Q_ReadStatus(void);
uint8_t W25Q_WaitBusy(void) ;
void W25Q_EraseSector(uint32_t addr) ;
void W25Q_Write(uint32_t addr, uint8_t *data, uint16_t len) ;
void W25Q_Read(uint32_t addr, uint8_t *data, uint16_t len) ;
#endif /* INC_FLASH_H_ */
