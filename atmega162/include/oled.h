#ifndef __OLED__
#define __OLED__

void OLED_init();

void OLED_goto_line(int line, int pageStart, int pageEnd);

void OLED_goto_column(int column, int colStart, int colEnd);

void OLED_set_pos(int row, int column);

void OLED_reset();

void OLED_write_char(char ch);

void OLED_print(char* str);

#endif // __OLED__