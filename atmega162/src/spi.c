#include <avr/interrupt.h>
#include <avr/io.h>

#include "spi.h"

#define MOSI_PIN PB5
#define SCK_PIN PB7

void SPI_init()
{
  /* Set MOSI and SCK output, all others input */
  DDRB |= (1 << SS_PIN) | (1 << MOSI_PIN) | (1 << SCK_PIN);

  /* Enable SPI, Master, set clock rate fck/16 */
  SPCR |= (1 << SPE) | (1 << MSTR) | (1 << SPR0);
}

void SPI_write_byte(char data)
{
  // start transmission
  SPDR = data;

  // wait for transmission complete
  while (!(SPSR & (1 << SPIF)))
    ;
}

char SPI_read_byte()
{
  // wait for reception complete
  while (!(SPSR & (1 << SPIF)))
    ;

  // return data register
  return SPDR;
}

void SPI_slave_select(slave_select_t status, int pin)
{
  if (status == SELECT)
    // slave select low
    PORTB &= ~(1 << pin);
  else
    // slave select high
    PORTB |= (1 << pin);
}
