#ifndef __SERVO__
#define __SERVO__

#include <stdint.h>

/**
 * Initialize servo controller
 */
void SERVO_init();

/**
 * Set servo position
 * @param angle angle to set the servo position (-100->100)
 */
void SERVO_set_pos(int8_t angle);

#endif // __SERVO__