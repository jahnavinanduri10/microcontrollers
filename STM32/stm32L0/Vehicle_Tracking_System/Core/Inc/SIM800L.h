/*
 * SIM800L.h
 */

#ifndef INC_SIM800L_H_
#define INC_SIM800L_H_

#include <stdint.h>

/* Send raw AT command and collect response into sim_buffer */
void SIM800_Send(char *cmd, uint32_t timeout);

/* Send an SMS message */
void SIM800_SendSMS(char *number, char *message);

/* Check GSM network registration; sets network_ok = 1 if registered */
void Check_Network_Status(void);

/* 1 = GSM network registered, 0 = no signal */
extern volatile uint8_t network_ok;

#endif /* INC_SIM800L_H_ */
