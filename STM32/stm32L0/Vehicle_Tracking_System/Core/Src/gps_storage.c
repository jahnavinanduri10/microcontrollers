/*
 * gps_storage.c
 *
 * Circular-buffer GPS location storage built on top of the W25Q flash
 * low-level drivers already present in flash.c (W25Q_Write / W25Q_Read /
 * W25Q_EraseSector).
 *
 * Meta sector layout (at FLASH_META_ADDR, 12 bytes):
 *   [0] uint8_t  head   – index of oldest entry (read pointer)
 *   [1] uint8_t  tail   – index of next write slot
 *   [2] uint8_t  count  – number of valid entries (0 … GPS_MAX_ENTRIES)
 *   [3] uint8_t  magic  – 0xA5 means meta is valid
 *
 * Data sector layout (at FLASH_DATA_ADDR):
 *   slot[n] = FLASH_DATA_ADDR + n * GPS_SLOT_SIZE   (64 bytes each)
 */

#include "main.h"
#include "flash.h"
#include "gps_storage.h"
#include <string.h>

/* ── Private state ──────────────────────────────────────────────────────── */
static uint8_t g_head;
static uint8_t g_tail;
static uint8_t g_count;

#define META_MAGIC  0xA5u

/* ── Private helpers ────────────────────────────────────────────────────── */

static uint32_t slot_addr(uint8_t idx)
{
    return FLASH_DATA_ADDR + (uint32_t)idx * GPS_SLOT_SIZE;
}

/** Persist head / tail / count to the meta sector. */
static void meta_save(void)
{
    uint8_t meta[4];
    meta[0] = g_head;
    meta[1] = g_tail;
    meta[2] = g_count;
    meta[3] = META_MAGIC;

    W25Q_EraseSector(FLASH_META_ADDR);
    W25Q_Write(FLASH_META_ADDR, meta, 4);
}

/* ── Public API ─────────────────────────────────────────────────────────── */

void GPS_Storage_Init(void)
{
    uint8_t meta[4] = {0};
    W25Q_Read(FLASH_META_ADDR, meta, 4);

    if (meta[3] == META_MAGIC)
    {
        /* Valid existing buffer state */
        g_head  = meta[0];
        g_tail  = meta[1];
        g_count = meta[2];

        /* Clamp to safe range in case of partial corruption */
        if (g_head  >= GPS_MAX_ENTRIES) g_head  = 0;
        if (g_tail  >= GPS_MAX_ENTRIES) g_tail  = 0;
        if (g_count >  GPS_MAX_ENTRIES) g_count = 0;
    }
    else
    {
        /* First boot — erase data sector and initialise meta */
        g_head  = 0;
        g_tail  = 0;
        g_count = 0;
        W25Q_EraseSector(FLASH_DATA_ADDR);
        meta_save();
    }
}

void GPS_Storage_Save(const char *location)
{
    /* Prepare a zero-padded slot */
    uint8_t buf[GPS_SLOT_SIZE];
    memset(buf, 0xFF, GPS_SLOT_SIZE);           /* 0xFF = erased flash state */
    uint16_t len = (uint16_t)strlen(location);
    if (len >= GPS_SLOT_SIZE) len = GPS_SLOT_SIZE - 1;
    memcpy(buf, location, len);
    buf[len] = '\0';

    /* If the slot we are about to write is already occupied we need to
       re-erase the data sector up to that slot.  Because W25Qxx erases
       in 4 KB sectors we keep all 5 slots inside the same sector and
       erase the whole sector only when wrapping around (tail == 0). */
    if (g_tail == 0)
    {
        W25Q_EraseSector(FLASH_DATA_ADDR);
    }

    W25Q_Write(slot_addr(g_tail), buf, GPS_SLOT_SIZE);

    /* Advance tail */
    g_tail = (g_tail + 1) % GPS_MAX_ENTRIES;

    if (g_count < GPS_MAX_ENTRIES)
    {
        g_count++;
    }
    else
    {
        /* Buffer full — advance head to drop oldest */
        g_head = (g_head + 1) % GPS_MAX_ENTRIES;
    }

    meta_save();
}

uint8_t GPS_Storage_ReadAll(char out[GPS_MAX_ENTRIES][GPS_SLOT_SIZE])
{
    uint8_t n = g_count;
    for (uint8_t i = 0; i < n; i++)
    {
        uint8_t idx = (g_head + i) % GPS_MAX_ENTRIES;
        W25Q_Read(slot_addr(idx), (uint8_t *)out[i], GPS_SLOT_SIZE);
        out[i][GPS_SLOT_SIZE - 1] = '\0';   /* ensure null-termination */
    }
    return n;
}

void GPS_Storage_Clear(void)
{
    g_head  = 0;
    g_tail  = 0;
    g_count = 0;
    W25Q_EraseSector(FLASH_DATA_ADDR);
    meta_save();
}

uint8_t GPS_Storage_Count(void)
{
    return g_count;
}
