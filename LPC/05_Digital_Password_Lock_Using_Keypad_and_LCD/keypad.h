#ifndef __KEYPAD_H__
#define __KEYPAD_H__
#include "types.h"

void initkpm(void);
u8 Rowcheck(void);
u8 colcheck(void);
u8 colscan(void);
u8 keyscan(void);
u32 Readnum(void);
void password(u8 *);

#endif
