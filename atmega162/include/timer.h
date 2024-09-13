#ifndef __TIMER__
#define __TIMER__

/**
 * Initialize hardware timers
 */
void TIMER_init();

/**
 * Set TIMER1 COMPA interrupt callback
 * @param cb pointer to new callback
 */
void TIMER_set_TIMER1_COMPA_cb(void (*cb)());

#endif // __TIMER__
