#include "main.h"

extern UART_HandleTypeDef huart1;

char sim_buffer[200];
volatile uint16_t idx = 0;
volatile uint8_t sms_ready = 0;
volatile uint8_t network_ok = 0;

void Check_Network_Status(void)
{
    SIM800_Send("AT+CREG?\r\n", 1000);

    if(strstr(sim_buffer, "+CREG: 0,1") ||
       strstr(sim_buffer, "+CREG: 0,5"))
    {
        network_ok = 1;
    }
    else
    {
        network_ok = 0;
    }
}

void SIM800_Send(char *cmd, uint32_t timeout)
{
    uint8_t ch;
    uint32_t start = HAL_GetTick();

    memset(sim_buffer, 0, sizeof(sim_buffer));
    idx = 0;

    // send command
    HAL_UART_Transmit(&huart1, (uint8_t*)cmd, strlen(cmd), HAL_MAX_DELAY);

    // read response
    while ((HAL_GetTick() - start) < timeout)
    {
        if (HAL_UART_Receive(&huart1, &ch, 1, 50) == HAL_OK)
        {
            if (idx < sizeof(sim_buffer) - 1)
                sim_buffer[idx++] = ch;
        }
    }

    sim_buffer[idx] = '\0';
    EPD_Print(sim_buffer);
}

void SIM800_SendSMS(char *number, char *message)
{
    char cmd[64];

    EPD_Print("Sending SMS");

    SIM800_Send("AT\r\n", 3000);

    SIM800_Send("AT+CMGF=1\r\n", 3000);

    sprintf(cmd, "AT+CMGS=\"%s\"\r\n", number);

    SIM800_Send(cmd, 5000);

    HAL_UART_Transmit(&huart1,(uint8_t*)message,strlen(message),HAL_MAX_DELAY);

    uint8_t ctrlZ = 0x1A;

    HAL_UART_Transmit(&huart1,&ctrlZ,1,HAL_MAX_DELAY);

    EPD_Print("SMS sent");
}

