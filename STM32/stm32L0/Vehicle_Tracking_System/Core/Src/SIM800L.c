/*
 * SIM800L.c
 *
 * GSM driver for SIM800L / SIM800C on UART1.
 * Baud rate: 9600 (SIM800L power-on default — more reliable than 115200
 * for long AT conversations on an STM32L0 at 32 MHz).
 */

#include "main.h"
#include "SIM800L.h"
#include "EPD.h"
#include <string.h>
#include <stdio.h>

extern UART_HandleTypeDef huart1;

/* Shared response buffer */
char             sim_buffer[256];
volatile uint16_t idx        = 0;
volatile uint8_t  network_ok = 0;   /* 1 = registered on network */

/* ── Low-level helper ────────────────────────────────────────────────────── */

/**
 * @brief  Send an AT command and collect the response within `timeout` ms.
 *         Result is stored in sim_buffer (null-terminated).
 */
void SIM800_Send(char *cmd, uint32_t timeout)
{
    uint8_t  ch;
    uint32_t start = HAL_GetTick();

    memset(sim_buffer, 0, sizeof(sim_buffer));
    idx = 0;

    /* Transmit command */
    HAL_UART_Transmit(&huart1, (uint8_t *)cmd, strlen(cmd), HAL_MAX_DELAY);

    /* Receive response until timeout */
    while ((HAL_GetTick() - start) < timeout)
    {
        if (HAL_UART_Receive(&huart1, &ch, 1, 10) == HAL_OK)
        {
            if (idx < sizeof(sim_buffer) - 1)
                sim_buffer[idx++] = (char)ch;
        }
    }
    sim_buffer[idx] = '\0';
}

/* ── Public API ──────────────────────────────────────────────────────────── */

/**
 * @brief  Query network registration status.
 *         Sets network_ok = 1 if the module is registered (home or roaming).
 *
 *  AT+CREG? response:
 *    +CREG: 0,1   → registered, home network
 *    +CREG: 0,5   → registered, roaming
 *    anything else → not registered
 */
void Check_Network_Status(void)
{
    SIM800_Send("AT+CREG?\r\n", 2000);

    if (strstr(sim_buffer, "+CREG: 0,1") ||
        strstr(sim_buffer, "+CREG: 0,5") ||
        strstr(sim_buffer, "+CREG: 1,1") ||
        strstr(sim_buffer, "+CREG: 1,5"))
    {
        network_ok = 1;
    }
    else
    {
        network_ok = 0;
    }
}

/**
 * @brief  Send an SMS message.
 * @param  number   Destination phone number string, e.g. "+918688558093"
 * @param  message  Text body (keep under ~160 chars for single SMS).
 */
void SIM800_SendSMS(char *number, char *message)
{
    char cmd[64];

    EPD_Print("Sending SMS...");

    /* Wake modem */
    SIM800_Send("AT\r\n", 1000);

    /* Text mode */
    SIM800_Send("AT+CMGF=1\r\n", 2000);

    /* Set recipient */
    snprintf(cmd, sizeof(cmd), "AT+CMGS=\"%s\"\r\n", number);
    SIM800_Send(cmd, 5000);

    /* Send body, terminated with Ctrl-Z (0x1A) */
    HAL_UART_Transmit(&huart1, (uint8_t *)message, strlen(message), HAL_MAX_DELAY);
    uint8_t ctrlZ = 0x1A;
    HAL_UART_Transmit(&huart1, &ctrlZ, 1, HAL_MAX_DELAY);

    /* Wait for "+CMGS:" confirmation (up to 10 s) */
    uint32_t start = HAL_GetTick();
    idx = 0;
    memset(sim_buffer, 0, sizeof(sim_buffer));
    uint8_t ch;
    while ((HAL_GetTick() - start) < 10000)
    {
        if (HAL_UART_Receive(&huart1, &ch, 1, 20) == HAL_OK)
        {
            if (idx < sizeof(sim_buffer) - 1)
                sim_buffer[idx++] = (char)ch;
            if (strstr(sim_buffer, "+CMGS:"))
                break;
        }
    }
    sim_buffer[idx] = '\0';

    EPD_Print("SMS sent");
}
