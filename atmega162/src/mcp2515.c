#include <avr/io.h>

#include "mcp2515_defines.h"
#include "spi.h"

void MCP2515_read(uint8_t addr, uint8_t *data_buffer, int buffer_size)
{
  SPI_slave_select(SELECT, SS_PIN);

  SPI_shift_data(MCP_READ);

  SPI_shift_data(addr);

  for (int i = 0; i < buffer_size; i++)
    data_buffer[i] = SPI_shift_data(DONT_CARE);

  SPI_slave_select(DESELECT, SS_PIN);
}

void MCP2515_write(uint8_t addr, uint8_t *data_buffer, int buffer_size)
{
  SPI_slave_select(SELECT, SS_PIN);

  SPI_shift_data(MCP_WRITE);

  SPI_shift_data(addr);

  for (int i = 0; i < buffer_size; i++)
    SPI_shift_data(data_buffer[i]);

  SPI_slave_select(DESELECT, SS_PIN);
}

void MCP2515_rts(int buffer_select_bits)
{
  SPI_slave_select(SELECT, SS_PIN);

  SPI_shift_data(buffer_select_bits);

  SPI_slave_select(DESELECT, SS_PIN);
}

uint8_t MCP2515_read_status()
{
  SPI_slave_select(SELECT, SS_PIN);

  SPI_shift_data(MCP_READ_STATUS);

  uint8_t status = SPI_shift_data(DONT_CARE);

  SPI_slave_select(DESELECT, SS_PIN);

  return status;
}

void MCP2515_bit_mod(uint8_t addr, uint8_t mask, uint8_t data)
{
  SPI_slave_select(SELECT, SS_PIN);

  SPI_shift_data(MCP_BITMOD);

  SPI_shift_data(addr);

  SPI_shift_data(mask);

  SPI_shift_data(data);

  SPI_slave_select(DESELECT, SS_PIN);
}

void MCP2515_reset()
{
  SPI_slave_select(SELECT, SS_PIN);

  SPI_shift_data(MCP_RESET);

  SPI_slave_select(DESELECT, SS_PIN);
}

void MCP2515_init()
{
  SPI_init();
  MCP2515_reset();
}
