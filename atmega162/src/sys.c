#include <avr/interrupt.h>
#include <avr/io.h>

#include "sys.h"

void SYS_init()
{
  cli();

  // Enable external memory
  MCUCR |= (1 << SRE);

  // Set memory address high bit mask
  SFIOR |= (4 << XMM0);

  // disable external interrupt INT0
  GICR &= ~(1 << INT0);

  sei();
}
