
#ifndef INC_HC04_ULTRASONIC_H_
#define INC_HC04_ULTRASONIC_H_



#include "main.h"

#include "stm32l0538_discovery.h"
#include "stm32l0538_discovery_epd.h"

#include <stdio.h>
#include <string.h>
#include <stdbool.h>


#define TRIG_PIN	GPIO_PIN_12
#define ECHO_PIN	GPIO_PIN_13
#define SENSOR_PORT	GPIOB


void HC04_Init(void);
void HC04_GetReading(void);
float HC04_GetTimes(char *buffer);

void Display_Init(void);
void Display_print(uint32_t entry,uint32_t exit);




#endif /* INC_HC04_ULTRASONIC_H_ */
