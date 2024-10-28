#include "sam.h"
#include "servo.h"
#include "pwm.h"

#define PWM_CPRD_20_ms 52500
#define PWM_CDTY_900_us 2363
#define PWM_CDTY_2100_us 5512

#define SERVO_ANGLE_TO_DUTY(angle) (-16 * angle + 3938)

void SERVO_init()
{
  PWM_init(PWM_CPRD_20_ms);
  PWM_start();
}

void SERVO_set_pos(int8_t angle)
{
  if (angle <= -100)
    PWM_set_duty_cycle(PWM_CDTY_2100_us);
  else if (angle >= 100)
    PWM_set_duty_cycle(PWM_CDTY_900_us);
  else
    PWM_set_duty_cycle(SERVO_ANGLE_TO_DUTY(angle));
}