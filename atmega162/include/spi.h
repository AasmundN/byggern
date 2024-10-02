#ifndef __SPI__
#define __SPI__

#define SS_PIN PB4

typedef enum
{
  SELECT,
  DESELECT
} slave_select_t;

void SPI_init();

void SPI_write_byte(char data);

char SPI_read_byte();

void SPI_slave_select(slave_select_t status, int pin);

#endif // __SPI__
