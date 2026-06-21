#include <lpc214x.h>
#include <stdio.h>
#include "distance.h"
#include "lcd.h"
#include "delay.h"
#include "spi_driver.h"

#define BASE_ADDR 0x0000


/* LPC2148 + HC-SR04 Ultrasonic Sensor (Register Level)
   Trig -> P0.0, Echo -> P0.1
   Author: Adapted from OCFreaks for your pin requirement
*/



#define PLOCK 0x00000400

void setupPLL0(void) {
    PLL0CON = 0x01; 
    PLL0CFG = 0x24;   // For 60MHz with 12MHz crystal
}

void feedSeq(void) {
    PLL0FEED = 0xAA;
    PLL0FEED = 0x55;
}

void connectPLL0(void) {
    while (!(PLL0STAT & PLOCK));
    PLL0CON = 0x03;
}

void initClocks(void) {
    setupPLL0();
    feedSeq();
    connectPLL0();
    feedSeq();
    VPBDIV = 0x01;    // PCLK = CCLK = 60MHz
}

// ------------------- Timer0 Functions -------------------
void initTimer0(void) {
    T0CTCR = 0x00;           // Timer Mode
    T0PR = 60 - 1;           // 60MHz / 60 = 1us resolution
    T0TCR = 0x02;            // Reset Timer
}

void delayUS(unsigned long us) {
    T0TCR = 0x02;            // Reset
    T0TCR = 0x01;            // Enable
    while (T0TC < us);
    T0TCR = 0x00;            // Disable
}

void delayMS(unsigned int ms) {
    delayUS(ms * 1000);
}

void startTimer0(void) {
    T0TCR = 0x02;            // Reset
    T0TCR = 0x01;            // Start
}

unsigned int stopTimer0(void) {
    T0TCR = 0x00;            // Stop
    return T0TC;
}

// ------------------- UART0 (for printf) -------------------
// Note: P0.0/P0.1 are used for sensor. If you need UART, move sensor to other pins
// or use different UART pins (e.g. P0.8/P0.9).

void initUART0(void) {
    // Example: Using P0.8 (Tx) & P0.9 (Rx) instead of P0.0/P0.1
    PINSEL0 |= (1<<18) | (1<<20);   // Select UART0 TxD0 & RxD0 on P0.9 & P0.8? Adjust if needed
    // Standard P0.0/P0.1 would be: PINSEL0 = 0x5; but we can't use them now.

    U0LCR = 0x83;          // 8-bit, 1 stop, DLAB=1
    U0DLL = 110;           // ~9600 baud @60MHz PCLK
    U0DLM = 1;
    U0FDR = (15<<4) | 1;   // Fractional divider
    U0LCR = 0x03;          // DLAB=0
}

int fputc(int c, FILE *stream) {   // Retarget printf
    while (!(U0LSR & (1<<5)));     // Wait for THRE
    U0THR = c;
    return c;
}

// ------------------- Main Program -------------------
#define TRIG (1<<0)   // P0.0
#define ECHO (1<<2)   // P0.1

int main(void) {
    unsigned int echoTime;
    float distance;
		char buf[20];
    initClocks();
    initTimer0();
   // initUART0();          // Optional - for serial output
		lcd_init();
    // GPIO Configuration
    PINSEL0 &= ~((3<<0) | (3<<4));   // P0.0 and P0.1 as GPIO (00)
    
    IO0DIR |= TRIG;       // P0.0 as OUTPUT
    IO0DIR &= ~ECHO;      // P0.1 as INPUT
    IO0CLR |= TRIG;       // Trig low initially
		lcd_str("SD logger");
		delay_ms(1000);
		lcd_cmd(0x01);
	
    while(1) {
        // Send 10us Trigger Pulse
        IO0SET |= TRIG;
        delayUS(10);
        IO0CLR |= TRIG;

        // Wait for Echo to go HIGH
        while (!(IO0PIN & ECHO));

        startTimer0();                    // Start timing

        // Wait for Echo to go LOW
        while (IO0PIN & ECHO);

        echoTime = stopTimer0();          // Time in microseconds

        distance = (0.0343f * echoTime) / 2.0f;   // Distance in cm
				sprintf(buf,"Distance :%f",distance);
				lcd_str(buf);
        delayMS(500);   // Update rate
    }
}