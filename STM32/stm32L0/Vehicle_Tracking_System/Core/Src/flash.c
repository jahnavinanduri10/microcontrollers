/*
 * flash.c
 *
 *  Created on: May 22, 2026
 *      Author: SANTOSH
 */

/*
 * w25qxx.c
 *
 *  Created on: May 19, 2026
 *      Author: Venu Madhav Anyam
 */
#include "main.h"
#include "flash.h"
#include "string.h"
extern SPI_HandleTypeDef hspi2;

#define FLASH_CS_PORT GPIOB
#define FLASH_CS_PIN  GPIO_PIN_12


void CS_LOW(void)
{
    HAL_GPIO_WritePin(FLASH_CS_PORT, FLASH_CS_PIN, GPIO_PIN_RESET);
}

void CS_HIGH(void)
{
    HAL_GPIO_WritePin(FLASH_CS_PORT, FLASH_CS_PIN, GPIO_PIN_SET);
}

/* -------- SPI HELPERS -------- */
void SPI_Write(uint8_t *data, uint16_t size) {
	HAL_SPI_Transmit(&hspi2, data, size, HAL_MAX_DELAY);
}

void SPI_Read(uint8_t *data, uint16_t size) {
	HAL_SPI_Receive(&hspi2, data, size, HAL_MAX_DELAY);
}

/* -------- FLASH -------- */
void W25Q_WriteEnable(void) {
	uint8_t cmd = 0x06;

	CS_LOW();
	HAL_SPI_Transmit(&hspi2, &cmd, 1, HAL_MAX_DELAY);
	CS_HIGH();
}

uint8_t W25Q_ReadStatus(void) {
	uint8_t tx[2] = { 0x05, 0xFF };
	uint8_t rx[2];

	CS_LOW();
	HAL_SPI_TransmitReceive(&hspi2, tx, rx, 2, 100);
	CS_HIGH();

	return rx[1];   // ⭐ correct byte
}

uint8_t W25Q_WaitBusy(void) {
	while (W25Q_ReadStatus() & 0x01)
		;
	return 0;
}

void W25Q_EraseSector(uint32_t addr) {
	uint8_t cmd[4];
	W25Q_WriteEnable();

	cmd[0] = 0x20;
	cmd[1] = (addr >> 16) & 0xFF;
	cmd[2] = (addr >> 8) & 0xFF;
	cmd[3] = addr & 0xFF;

	CS_LOW();
	SPI_Write(cmd, 4);
	CS_HIGH();

	W25Q_WaitBusy();
}

void W25Q_Write(uint32_t addr, uint8_t *data, uint16_t len) {
	W25Q_WriteEnable();

	uint8_t cmd[4];
	cmd[0] = 0x02;
	cmd[1] = (addr >> 16) & 0xFF;
	cmd[2] = (addr >> 8) & 0xFF;
	cmd[3] = addr & 0xFF;

	CS_LOW();

	HAL_SPI_Transmit(&hspi2, cmd, 4, HAL_MAX_DELAY);
	HAL_SPI_Transmit(&hspi2, data, len, HAL_MAX_DELAY);

	CS_HIGH();

	W25Q_WaitBusy();
}

void W25Q_Read(uint32_t addr, uint8_t *data, uint16_t len) {
	uint8_t cmd[4];

	cmd[0] = 0x03;
	cmd[1] = (addr >> 16) & 0xFF;
	cmd[2] = (addr >> 8) & 0xFF;
	cmd[3] = addr & 0xFF;
	memset(data,0xFF,len);
	CS_LOW();

	HAL_SPI_Transmit(&hspi2, cmd, 4, HAL_MAX_DELAY);

	HAL_SPI_Receive(&hspi2, data, len, HAL_MAX_DELAY);

	CS_HIGH();
}

