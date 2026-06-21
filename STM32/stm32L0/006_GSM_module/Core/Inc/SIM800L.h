#ifndef INC_SIM800L_H_
#define INC_SIM800L_H_

void SIM800_Send(char *cmd, uint32_t timeout);
void SIM800_SendSMS(char *number, char *message);

#endif /* INC_SIM800L_H_ */
