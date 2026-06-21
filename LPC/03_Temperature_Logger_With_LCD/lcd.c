#include <lpc214x.h>
#include "lcd.h"

#define LCD_RS (1<<16)   // P1.16
#define LCD_EN (1<<17)   // P1.17
#define LCD_D4 (1<<18)   // P1.18
#define LCD_D5 (1<<19)   // P1.19
#define LCD_D6 (1<<20)   // P1.20
#define LCD_D7 (1<<21)   // P1.21

static void delay_ms(unsigned int ms)
{
    unsigned int i,j;

    for(i=0;i<ms;i++)
        for(j=0;j<6000;j++);
}

static void lcd_pulse(void)
{
    IOSET1 = LCD_EN;
    delay_ms(2);

    IOCLR1 = LCD_EN;
    delay_ms(2);
}

static void lcd_send4(unsigned char nibble)
{
    IOCLR1 = LCD_D4 | LCD_D5 | LCD_D6 | LCD_D7;

    if(nibble & 0x01) IOSET1 = LCD_D4;
    if(nibble & 0x02) IOSET1 = LCD_D5;
    if(nibble & 0x04) IOSET1 = LCD_D6;
    if(nibble & 0x08) IOSET1 = LCD_D7;

    lcd_pulse();
}

void lcd_cmd(unsigned char cmd)
{
    IOCLR1 = LCD_RS;

    lcd_send4(cmd >> 4);
    lcd_send4(cmd & 0x0F);

    delay_ms(2);
}

void lcd_data(unsigned char data)
{
    IOSET1 = LCD_RS;

    lcd_send4(data >> 4);
    lcd_send4(data & 0x0F);

    delay_ms(2);
}

void lcd_init(void)
{
    IODIR1 |= LCD_RS | LCD_EN | LCD_D4 | LCD_D5 | LCD_D6 | LCD_D7;

    delay_ms(20);

    lcd_send4(0x03);
    delay_ms(5);

    lcd_send4(0x03);
    delay_ms(5);

    lcd_send4(0x03);
    delay_ms(5);

    lcd_send4(0x02);

    lcd_cmd(0x28);   // 4-bit, 2-line
    lcd_cmd(0x0C);   // display ON
    lcd_cmd(0x06);   // entry mode
    lcd_cmd(0x01);   // clear

    delay_ms(5);
}

void lcd_str(const char *str)
{
    while(*str)
    {
        lcd_data(*str++);
    }
}