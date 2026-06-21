#ifndef __RTC_H__
#define __RTC_H__
#include "types.h"

void RTC_Init(void);
void RTCSetTime(u32,u32,u32);
void RTC_GetTime(u32 *hr, u32 *min, u32 *sec);
#endif
