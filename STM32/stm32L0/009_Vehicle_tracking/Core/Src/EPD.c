#include "main.h"

void EPD_Init(void)
{
	BSP_EPD_Init();
	BSP_EPD_Clear(EPD_COLOR_WHITE);
	BSP_EPD_SetFont(&Font16);
}

void EPD_Print(char *msg)
{
    BSP_EPD_Clear(EPD_COLOR_WHITE);

    BSP_EPD_DisplayStringAt(5,4,(uint8_t*)msg,LEFT_MODE);

    BSP_EPD_RefreshDisplay();
}


