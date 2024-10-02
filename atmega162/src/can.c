#include <avr/io.h>

#include "can.h"
#include "spi.h"

// controller command IDs
#define WRITE_CMD 0x2
#define READ_CMD 0x3

void CAN_init() { SPI_init(); }

void CAN_controller_read(char addr, char data_buffer[], int buffer_size)
{
  SPI_slave_select(SELECT, SS_PIN);

  SPI_write_byte(READ_CMD);

  SPI_write_byte(addr);

  for (int i = 0; i < buffer_size; i++)
  {
    data_buffer[i] = SPI_read_byte();
  }

  SPI_slave_select(DESELECT, SS_PIN);
}

void CAN_controller_write(char addr, char data_buffer[], int buffer_size)
{
  SPI_slave_select(SELECT, SS_PIN);

  SPI_write_byte(WRITE_CMD);

  SPI_write_byte(addr);

  for (int i = 0; i < buffer_size; i++)
    SPI_write_byte(data_buffer[i]);

  SPI_slave_select(DESELECT, SS_PIN);
}
