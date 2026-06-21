
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include <string.h>
#include <stdio.h>

#include "stm32l0xx_hal.h"
#include "stm32l0xx_hal_tim.h"
#include "stm32l0538_discovery.h"
#include "stm32l0538_discovery_epd.h"

#include "EPD.h"
#include "SIM800L.h"


/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

#ifdef __cplusplus
}
#endif

