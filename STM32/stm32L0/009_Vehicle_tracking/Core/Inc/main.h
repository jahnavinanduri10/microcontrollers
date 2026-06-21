
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32l0xx_hal.h"

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

#include <string.h>
#include <stdio.h>

#include "stm32l0xx_hal.h"
#include "stm32l0xx_hal_tim.h"
#include "stm32l0538_discovery.h"
#include "stm32l0538_discovery_epd.h"

#include "EPD.h"
#include "SIM800L.h"

#define SLAVE_ADDR 0x08<<1

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
