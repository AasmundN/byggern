#include <avr/io.h>
#include <util/delay.h>

#include "oled.h"
#include "fonts.h"

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


void OLED_init()
{
  OLED_CMD_REG = CMD_DISP_OFF;
  OLED_CMD_REG = CMD_SEGMENT_REMAP;

  OLED_CMD_REG = CMD_SET_COM_PINS;
  OLED_CMD_REG = 0x12; // alternative COM pin config

  OLED_CMD_REG = CMD_SET_COM_OUTPUT_SCAN_DIR; // inverted

  OLED_CMD_REG = CMD_SET_MULTIPLEX_RATIO; // ration mode:63
  OLED_CMD_REG = 0x3f; // 63d, resulting in 64MUX

  OLED_CMD_REG = CMD_SET_DISP_CLOCK_AND_OSC_FREQ;
  OLED_CMD_REG = 0x80; // divide ratio set to default

  OLED_CMD_REG = CMD_SET_CONTRAST_CONTROL;
  OLED_CMD_REG = 0x50; // contrast value

  OLED_CMD_REG = CMD_SET_PRE_CHARGE_PERIOD;
  OLED_CMD_REG = 0x21; // 0x1 = 1 DCLK cycle, 0x2 = 2 DCLK cycles

  OLED_CMD_REG = CMD_SET_MEMORY_ADDR_MODE;
  OLED_CMD_REG = 0x00; // horizontal addressing mode

  OLED_CMD_REG = CMD_SET_VCOM_DESELECT_LEVEL_MODE;
  OLED_CMD_REG = 0x30; // 0.83*Vcc

  OLED_CMD_REG = CMD_SET_EXT_OR_INT_IREF;
  OLED_CMD_REG = 0x00; // disable external Vcc supply

  OLED_CMD_REG = CMD_SET_DISP_RAM;
  OLED_CMD_REG = CMD_SET_NORMAL_DISP;
  OLED_CMD_REG = CMD_DISP_ON;
}

void OLED_goto_line(int line, int page_start, int page_end){
  OLED_CMD_REG = CMD_SET_PAGE_ADDR; 
  OLED_CMD_REG = (0b00000111 & page_start);
  OLED_CMD_REG = (0b00000111 & page_end);
}

void OLED_goto_column(int column, int col_start, int col_end){
  OLED_CMD_REG = CMD_SET_COL_ADDR;
  OLED_CMD_REG = (0b01111111 & col_start);
  OLED_CMD_REG = (0b01111111 & col_end);
}

void OLED_set_pos(int line, int column){
  OLED_goto_line(line, 0, 7);
  OLED_goto_column(column, 0, 127);
}

void OLED_reset(){
  OLED_set_pos(0,0);
  for(int i=0; i<128*8; i++){
    OLED_DATA_REG = 0x00;
  }
}

void OLED_write_char(char ch) {
  for (int i = 0; i < 8; i++)
    OLED_DATA_REG = pgm_read_byte(&(font8[ch - 32][i]));
}

void OLED_print(char* str){
  int i = 0;
  while (str[i] != '\0')
    OLED_write_char(str[i++]);
}