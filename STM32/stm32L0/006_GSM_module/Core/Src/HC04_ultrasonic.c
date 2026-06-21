#include "HC04_Ultrasonic.h"

volatile float distance = 0;

#define EMPTY_DISTANCE   18.0f
#define MAX_DISTANCE     50.0f

void HC04_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    __HAL_RCC_GPIOB_CLK_ENABLE();

    HAL_GPIO_WritePin(SENSOR_PORT, TRIG_PIN, GPIO_PIN_RESET);

    GPIO_InitStruct.Pin   = TRIG_PIN;
    GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull  = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(SENSOR_PORT, &GPIO_InitStruct);

    GPIO_InitStruct.Pin  = ECHO_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(SENSOR_PORT, &GPIO_InitStruct);
}

static void delay_us(uint16_t us)
{
    volatile uint32_t count;
    while(us--)
    {
        count = 11;
        while(count--);
    }
}

void HC04_GetReading(void)
{
    uint32_t time    = 0;
    uint32_t timeout = 30000;

    HAL_Delay(50);

    // Trigger pulse
    HAL_GPIO_WritePin(SENSOR_PORT, TRIG_PIN, GPIO_PIN_RESET);
    delay_us(2);
    HAL_GPIO_WritePin(SENSOR_PORT, TRIG_PIN, GPIO_PIN_SET);
    delay_us(10);
    HAL_GPIO_WritePin(SENSOR_PORT, TRIG_PIN, GPIO_PIN_RESET);

    // Wait for ECHO to go HIGH — with timeout
    timeout = 30000;
    while(HAL_GPIO_ReadPin(SENSOR_PORT, ECHO_PIN) == GPIO_PIN_RESET)
    {
        if(--timeout == 0) { distance = 0; return; }
        delay_us(1);
    }

    // Measure ECHO HIGH time — with timeout
    time    = 0;
    timeout = 30000;
    while(HAL_GPIO_ReadPin(SENSOR_PORT, ECHO_PIN) == GPIO_PIN_SET)
    {
        time++;
        delay_us(1);
        if(--timeout == 0) { distance = 0; return; }
    }

    distance = time / 58.0f;
}

// Returns true if an object is in the detection zone
static bool object_present(void)
{
    // Object is present only if it is CLOSER than the empty-scene distance
    return (distance > 2.0f && distance < EMPTY_DISTANCE);
}

float HC04_GetTimes(char *buffer)
{
    uint32_t entry_time, exit_time;
    float entry_distance = 0.0f;

    HC04_GetReading();

    if(!object_present())
    {
        sprintf(buffer, "[SENSOR] : No Object\r\n");
        Display_print(0, 0);
    }
    else
    {
        entry_distance = distance;
        entry_time     = HAL_GetTick();

        // Wait for object to leave — require 5 consecutive clear readings
        // to avoid exiting on a single noisy reading
        uint8_t clear_count = 0;
        while(clear_count < 5)
        {
            HC04_GetReading();
            if(object_present())
                clear_count = 0;  // object still there, reset
            else
                clear_count++;    // one more clear reading
        }

        exit_time = HAL_GetTick();
        sprintf(buffer, "[SENSOR] : Object entered:%lu exit:%lu\r\n",
                entry_time, exit_time);
        Display_print(entry_time, exit_time);
    }

    return entry_distance;
}

void Display_print(uint32_t entry, uint32_t exit)
{
    char buffer[30];
    BSP_EPD_Clear(EPD_COLOR_WHITE);

    if(entry == 0 && exit == 0)
    {
        BSP_EPD_DisplayStringAt(5, 11, (uint8_t *)"No Object", LEFT_MODE);
        BSP_EPD_RefreshDisplay();
    }
    else
    {
        sprintf(buffer, "Entered:%lu", entry);
        BSP_EPD_DisplayStringAt(5, 11, (uint8_t *)buffer, LEFT_MODE);
        sprintf(buffer, "Exit:%lu", exit);
        BSP_EPD_DisplayStringAt(5, 7,  (uint8_t *)buffer, LEFT_MODE);
        BSP_EPD_RefreshDisplay();
    }
}

void Display_Init(void)
{
    char *msg = "Display Init...";
    BSP_EPD_Init();
    BSP_LED_Init(LED3);
    BSP_EPD_Clear(EPD_COLOR_WHITE);
    BSP_EPD_SetFont(&Font16);
    BSP_EPD_DisplayStringAt(5, 11, (uint8_t *)msg, LEFT_MODE);
    BSP_EPD_RefreshDisplay();
    HAL_Delay(1500);
    BSP_EPD_Clear(EPD_COLOR_WHITE);
    BSP_EPD_RefreshDisplay();
}
