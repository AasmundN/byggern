#ifndef __MOTOR__
#define __MOTOR__

#include <stdint.h>

/**
 * Initialize the motor controller
 */
void MOTOR_init();

/**
 * Set the motor position
 * @param pos motor position to set (0->100)
 */
void MOTOR_set_pos(int8_t pos);

#endif // __MOTOR__