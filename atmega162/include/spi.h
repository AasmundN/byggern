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

/**
 * Shift data on SPI bus
 * @param data to shift out
 * @return data shifted in
 */
char SPI_shift_data(char data);

/**
 * Pull slave select high/low
 * @param select high/low
 * @param pin offset in PORTB
 */
void SPI_slave_select(slave_select_t status, int pin);

#endif // __SPI__
