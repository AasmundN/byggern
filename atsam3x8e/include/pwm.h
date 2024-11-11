#ifndef __PWM__
#define __PWM__

#include <stdint.h>

typedef enum
{
  SERVO_PWM,
  MOTOR_PWM
} pwm_signal_t;

/**
 * Initialize the PWM signal
 * @param pwm_signal type of PWM signal to initialize
 * @param period period of the PWM signal
 */
void PWM_init(pwm_signal_t pwm_signal, uint16_t period);

/**
 * Stop PWM signal
 * @param pwm_signal type of PWM signal to stop
 */
void PWM_stop(pwm_signal_t pwm_signal);

/**
 * Start the PWM signal
 * @param pwm_signal type of PWM signal to start
 */
void PWM_start(pwm_signal_t pwm_signal);

/**
 * Set the duty cycle of the PWM signal
 * @param pwm_signal type of PWM signal
 * @param duty_cycle duty cycle to set
 */
void PWM_set_duty_cycle(pwm_signal_t pwm_signal, uint16_t duty_cycle);

#endif // __PWM__
