#ifndef __TIMER__
#define __TIMER__

#include <stdint.h>

typedef struct time
{
  uint8_t minutes;
  uint8_t seconds;
} time_t;

/**
 * Initialize hardware timers
 */
void TIMER_init();

/**
 * Set TIMER1 COMPA interrupt callback
 * @param cb pointer to new callback
 */
void TIMER_set_TIMER1_COMPA_cb(void (*cb)());

/**
 * Reset game timer
 */
void TIMER_reset_game_timer();

/**
 * Fetch game timer value
 * @return time since game timer reset
 */
time_t TIMER_get_game_time();

#endif // __TIMER__
