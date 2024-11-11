#include <avr/interrupt.h>
#include <avr/io.h>

#include "timer.h"

time_t game_timer = {
    .minutes = 0,
    .seconds = 0,
};

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
  OCR1A = 4600;

  // TIMER1: compare A interrupt enable
  TIMSK |= (1 << OCIE1A);

  TIMER1_COMPA_cb = default_TIMER1_COMPA_cb;

  sei();
}

void TIMER_set_TIMER1_COMPA_cb(void (*cb)()) { TIMER1_COMPA_cb = cb; }

ISR(TIMER1_COMPA_vect)
{
  TIMER1_COMPA_cb();

  game_timer.seconds++;

  if (game_timer.seconds < 60)
    return;

  game_timer.seconds = 0;
  game_timer.minutes++;
}

void TIMER_start_game_timer()
{
  game_timer.minutes = 0;
  game_timer.seconds = 0;
}

time_t TIMER_get_game_time() { return game_timer; }
