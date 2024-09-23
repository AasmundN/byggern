#ifndef __OLED__
#define __OLED__

#define OLED_CMD_REG (*(volatile uint8_t*)0x1000)
#define OLED_DATA_REG (*(volatile uint8_t*)0x1200)

#define CMD_DISP_OFF 0xae
#define CMD_SEGMENT_REMAP 0xa1
#define CMD_SET_COM_PINS 0xda
#define CMD_SET_COM_OUTPUT_SCAN_DIR 0xc8
#define CMD_SET_MULTIPLEX_RATIO 0xa8
#define CMD_SET_DISP_CLOCK_AND_OSC_FREQ 0xd5
#define CMD_SET_CONTRAST_CONTROL 0x81
#define CMD_SET_PRE_CHARGE_PERIOD 0xd9
#define CMD_SET_MEMORY_ADDR_MODE 0x20
#define CMD_SET_VCOM_DESELECT_LEVEL_MODE 0xdb
#define CMD_SET_EXT_OR_INT_IREF 0xad
#define CMD_SET_DISP_RAM 0xa4
#define CMD_SET_NORMAL_DISP 0xa6
#define CMD_DISP_ON 0xaf

#define CMD_SET_PAGE_ADDR 0x22
#define CMD_SET_COL_ADDR 0x21


void OLED_init();

void OLED_goto_line(int line, int pageStart, int pageEnd);

void OLED_goto_column(int column, int colStart, int colEnd);

void OLED_clear_line(int line);

void OLED_set_pos(int row, int column);

void OLED_reset();

void OLED_write_char(char ch);

void OLED_print(char* str);

#endif // __OLED__