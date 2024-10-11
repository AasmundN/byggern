#ifndef __SPI__
#define __SPI__

#define SS_PIN PB4
#define DONT_CARE 0

typedef enum
{
  SELECT,
  DESELECT
} slave_select_t;

void SPI_init();

char SPI_shift_data(char data);

void SPI_slave_select(slave_select_t status, int pin);

#endif // __SPI__
