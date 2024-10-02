#include "sys.h"
#include <avr/io.h>

void SYS_init()
{
  // Enable external memory
  MCUCR |= (1 << SRE);

  // Set memory address high bit mask
  SFIOR |= (4 << XMM0);
}
