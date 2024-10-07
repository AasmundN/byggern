#include <avr/io.h>

#include "MCP2515.h"
#include "spi.h"

// controller command IDs
#define WRITE_CMD 0x2
#define READ_CMD 0x3
#define RTS_CMD_BASE 0x80
#define READ_STATUS_CMD 0xa0
#define BIT_MOD_CMD 0x5
#define RESET_CMD 0xc0

void MCP2515_init()
{
  SPI_init();
  MCP2515_reset();
}

void MCP2515_read(char addr, char data_buffer[], int buffer_size)
{
  SPI_slave_select(SELECT, SS_PIN);

  SPI_shift_data(READ_CMD);

  SPI_shift_data(addr);

  for (int i = 0; i < buffer_size; i++)
    data_buffer[i] = SPI_shift_data(DONT_CARE);

  SPI_slave_select(DESELECT, SS_PIN);
}

void MCP2515_write(char addr, char data_buffer[], int buffer_size)
{
  SPI_slave_select(SELECT, SS_PIN);

  SPI_shift_data(WRITE_CMD);

  SPI_shift_data(addr);

  for (int i = 0; i < buffer_size; i++)
    SPI_shift_data(data_buffer[i]);

  SPI_slave_select(DESELECT, SS_PIN);
}

void MCP2515_rts(int buffer_select_bits)
{
  SPI_slave_select(SELECT, SS_PIN);

  SPI_shift_data(RTS_CMD_BASE | buffer_select_bits);

  SPI_slave_select(DESELECT, SS_PIN);
}

char MCP2515_read_status()
{
  SPI_slave_select(SELECT, SS_PIN);

  SPI_shift_data(READ_STATUS_CMD);

  char status = SPI_shift_data(DONT_CARE);

  SPI_slave_select(DESELECT, SS_PIN);

  return status;
}

void MCP2515_bit_mod(char addr, char mask, char data)
{
  SPI_slave_select(SELECT, SS_PIN);

  SPI_shift_data(BIT_MOD_CMD);

  SPI_shift_data(addr);

  SPI_shift_data(mask);

  SPI_shift_data(data);

  SPI_slave_select(DESELECT, SS_PIN);
}

void MCP2515_reset()
{
  SPI_slave_select(SELECT, SS_PIN);

  SPI_shift_data(RESET_CMD);

  SPI_slave_select(DESELECT, SS_PIN);
}
