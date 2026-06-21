#ifndef __DEFINES__
#define __DEFINES__

#define SETBIT(port,pin) port |= ( 1 << pin )
#define CLRBIT(port,pin) port &= ~(1 << pin )


#endif
