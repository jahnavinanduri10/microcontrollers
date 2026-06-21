/*
 * gps_storage.h
 *
 * Circular buffer storage of GPS locations in W25Qxx Flash.
 * Stores up to GPS_MAX_ENTRIES valid location strings.
 * Each slot = GPS_SLOT_SIZE bytes, starting at FLASH_BASE_ADDR.
 *
 * Flash layout (4 KB sector at 0x000000):
 *   [0x000]  Meta sector (head, tail, count) - 12 bytes
 *   [0x001000 .. 0x001000 + GPS_MAX_ENTRIES*GPS_SLOT_SIZE]  Data slots
 *
 * The meta sector is erased & rewritten on every update.
 * Data sector: erased once at init if first boot, then slots written
 * individually (each slot fits in one page-program, W25Q page = 256 bytes).
 */

#ifndef INC_GPS_STORAGE_H_
#define INC_GPS_STORAGE_H_

#include <stdint.h>

/* ── Configuration ──────────────────────────────────────────────────────── */
#define GPS_MAX_ENTRIES    5          /* circular buffer depth               */
#define GPS_SLOT_SIZE      64         /* bytes per location entry            */

/* Flash byte addresses */
#define FLASH_META_ADDR    0x000000UL /* 4 KB sector for head/tail/count     */
#define FLASH_DATA_ADDR    0x001000UL /* 4 KB sector for location slots      */

/* ── Public API ─────────────────────────────────────────────────────────── */

/**
 * @brief  Initialise the GPS storage subsystem.
 *         Reads existing head/tail/count from flash meta sector.
 *         Call once after W25Q SPI is ready.
 */
void GPS_Storage_Init(void);

/**
 * @brief  Save a GPS location string to the circular buffer.
 *         If buffer is full the oldest entry is overwritten.
 * @param  location  Null-terminated string, max (GPS_SLOT_SIZE-1) chars.
 *                   Example: "17.3850,78.4867"
 */
void GPS_Storage_Save(const char *location);

/**
 * @brief  Read back all stored locations in FIFO order.
 * @param  out       2-D array of GPS_MAX_ENTRIES x GPS_SLOT_SIZE bytes.
 * @return Number of valid entries read (0 … GPS_MAX_ENTRIES).
 */
uint8_t GPS_Storage_ReadAll(char out[GPS_MAX_ENTRIES][GPS_SLOT_SIZE]);

/**
 * @brief  Clear the circular buffer (erase both meta and data sectors).
 *         Call after all stored locations have been sent via SMS.
 */
void GPS_Storage_Clear(void);

/**
 * @brief  Return the number of locations currently buffered.
 */
uint8_t GPS_Storage_Count(void);

#endif /* INC_GPS_STORAGE_H_ */
