#include <avr/interrupt.h>
#include <avr/io.h>

#include "timer.h"

void (*TIMER1_COMPA_cb)();

void default_TIMER1_COMPA_cb() {}

void TIMER_init()
{
  cli();

  // TIMER1: CTC mode, prescaler 1024
  TCCR1B |= (1 << WGM12) | (1 << CS12) | (1 << CS10);

  // TIMER1: reset count
  TCNT1 = 0;

  // TIMER1: count to roughly 750ms
  OCR1A = 3520;

  // TIMER1: compare A interrupt enable
  TIMSK |= (1 << OCIE1A);

  TIMER1_COMPA_cb = default_TIMER1_COMPA_cb;

  sei();
}

void TIMER_set_TIMER1_COMPA_cb(void (*cb)()) { TIMER1_COMPA_cb = cb; }

ISR(TIMER1_COMPA_vect) { TIMER1_COMPA_cb(); }
