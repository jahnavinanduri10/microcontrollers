#include <lpc214x.h>
#include "lcd.h"

#define DIR 		IODIR0
#define SET0		IOSET0
#define CLR0		IOCLR0

#define LCD_RS (1<<16)
#define LCD_EN (1<<17)
#define LCD_D4 (1<<18)
#define LCD_D5 (1<<19)
#define LCD_D6 (1<<20)
#define LCD_D7 (1<<21)

static void delay_ms(unsigned int ms)
{
    unsigned int i, j;
    for(i = 0; i < ms; i++)
        for(j = 0; j < 6000; j++);
}

static void lcd_pulse(void)
{
    SET0 = LCD_EN;
    delay_ms(2);
    CLR0 = LCD_EN;
}

static void lcd_send4(unsigned char nibble)
{
    if(nibble & 0x01) SET0 = LCD_D4; else CLR0 = LCD_D4;
    if(nibble & 0x02) SET0 = LCD_D5; else CLR0 = LCD_D5;
    if(nibble & 0x04) SET0 = LCD_D6; else CLR0 = LCD_D6;
    if(nibble & 0x08) SET0 = LCD_D7; else CLR0 = LCD_D7;
    lcd_pulse();
}

void lcd_cmd(unsigned char cmd)
{
    CLR0 = LCD_RS;
    lcd_send4(cmd >> 4);
    lcd_send4(cmd & 0x0F);
    delay_ms(2);
}

void lcd_data(unsigned char data)
{
    SET0 = LCD_RS;
    lcd_send4(data >> 4);
    lcd_send4(data & 0x0F);
    delay_ms(2);
}

void lcd_init(void)
{
    DIR |= LCD_RS | LCD_EN | LCD_D4 | LCD_D5 | LCD_D6 | LCD_D7;
    delay_ms(20);
    lcd_send4(0x03);
    delay_ms(5);
    lcd_send4(0x03);
    delay_ms(1);
    lcd_send4(0x03);
    lcd_send4(0x02);

    lcd_cmd(0x28);
    lcd_cmd(0x0C);
    lcd_cmd(0x06);
    lcd_cmd(0x01);
}

void lcd_str(const char *str)
{
    while(*str)
        lcd_data(*str++);
}
