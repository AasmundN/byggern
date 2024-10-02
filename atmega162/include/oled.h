#ifndef __OLED__
#define __OLED__


//Amogus symbol
#define MOGUS 0

/**
 * Initialize the OLED module
 */
void OLED_init();


/**
 * Write character to specified position
 * @param ch character to be written
 * @param row index of the row
 * @param column index of the column
 */
void OLED_write_char(char ch, int row, int column);

/**
 * Write string to specified position
 * @param str sring to be written
 * @param row index of the row
 * @param column index of the column
 */
void OLED_print(char* str, int row, int column);

/**
 * Clear the specified line
 * @param row index of the row (line) to be cleared
 */
void OLED_clear_line(int row);

/**
 * Clear the entire screen
 */
void OLED_clear_screen();

/**
 * Load frame from SRAM onto the OLED
 */
void OLED_refresh();

#endif // __OLED__