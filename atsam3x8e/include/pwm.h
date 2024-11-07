#ifndef __PWM__
#define __PWM__

#include <stdint.h>

typedef enum
{
  SERVO_PWM,
  MOTOR_PWM
} pwm_signal_t;

void PWM_init(pwm_signal_t pwm_signal, uint16_t period);

void PWM_stop(pwm_signal_t pwm_signal);
void PWM_start(pwm_signal_t pwm_signal);

void PWM_set_duty_cycle(pwm_signal_t pwm_signal, uint16_t duty_cycle);

#endif // __PWM__
