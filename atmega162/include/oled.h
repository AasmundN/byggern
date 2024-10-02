#ifndef __OLED__
#define __OLED__

void OLED_init();

void OLED_write_char(char ch, int row, int column);

void OLED_print(char* str, int row, int column);

void OLED_clear();

void OLED_refresh();

#endif // __OLED__