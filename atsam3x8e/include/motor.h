#ifndef __MOTOR__
#define __MOTOR__

#include <stdbool.h>
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

/**
 * Start motor controller
 */
void MOTOR_start();

/**
 * Stop motor controller
 */
void MOTOR_stop();

#endif // __MOTOR__
