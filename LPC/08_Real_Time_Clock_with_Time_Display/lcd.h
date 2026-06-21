#ifndef __LCD_H__
#define __LCD_H__

void lcd_init(void);
void lcd_cmd(unsigned char cmd);
void lcd_data(unsigned char data);
void lcd_str(const char *str);

#endif
