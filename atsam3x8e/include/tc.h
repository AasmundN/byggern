#ifndef __TC__
#define __TC__

/**
 * Initialize timer counter
 * @param period period of the timer counter
 */
void TC_init(unsigned long period);

/**
 * Set the callback function for the timer counter
 * @param cb pointer to the callback function
 */
void TC_set_cb(void (*cb)());

#endif // __TC__