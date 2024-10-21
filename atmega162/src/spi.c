#include <avr/interrupt.h>
#include <avr/io.h>

#include "spi.h"

#define MOSI_PIN PB5
#define MISO_PIN PB6
#define SCK_PIN PB7

void SPI_init()
{
  // set SS, MOSI, SCK output
  DDRB |= (1 << SS_PIN) | (1 << MOSI_PIN) | (1 << SCK_PIN);

  // set MISO input
  DDRB &= ~(1 << MISO_PIN);

  // enable SPI, Master, set clock rate fck/16
  SPCR |= (1 << SPE) | (1 << MSTR) | (1 << SPR0);
}

uint8_t SPI_shift_data(uint8_t data)
{
  // start transmission
  SPDR = data;

  // wait for transmission complete
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
  else if (status == DESELECT)
    // slave select high
    PORTB |= (1 << pin);
}
