#include "pwm.h"
#include "sam.h"

void PWM_init(pwm_signal_t pwm_signal, uint16_t period)
{
  REG_PMC_PCER1 |= PMC_PCER1_PID36; // Enable PWM in PMC enable register.

  if (pwm_signal == SERVO_PWM)
  {
    REG_PIOB_PDR |= PIO_PB13B_PWMH1;  // Disable PIO control
    REG_PIOB_ABSR |= PIO_PB13B_PWMH1; // Select PWMH1 peripheral B

    REG_PWM_CMR1 |= PWM_CMR_CPRE_MCK_DIV_32 | PWM_CMR_CPOL; // MCK/32 clock
    REG_PWM_CPRD1 |= PWM_CPRD_CPRD(period);
    REG_PWM_CDTY1 |= PWM_CDTY_CDTY(0);
  }
  else if (pwm_signal == MOTOR_PWM)
  {
    REG_PIOB_PDR |= PIO_PB12B_PWMH0;  // Disable PIO control
    REG_PIOB_ABSR |= PIO_PB12B_PWMH0; // Select PWMH0 peripheral B

    REG_PWM_CMR0 |= PWM_CMR_CPRE_MCK_DIV_32 | PWM_CMR_CPOL; // MCK/32 clock
    REG_PWM_CPRD0 |= PWM_CPRD_CPRD(period);
    REG_PWM_CDTY0 |= PWM_CDTY_CDTY(0);
  }
}

void PWM_stop(pwm_signal_t pwm_signal)
{
  if (pwm_signal == SERVO_PWM)
    REG_PWM_DIS |= PWM_DIS_CHID1;
  else if (pwm_signal == MOTOR_PWM)
    REG_PWM_DIS |= PWM_DIS_CHID0;
}

void PWM_start(pwm_signal_t pwm_signal)
{
  if (pwm_signal == SERVO_PWM)
    REG_PWM_ENA |= PWM_ENA_CHID1;
  else if (pwm_signal == MOTOR_PWM)
    REG_PWM_ENA |= PWM_ENA_CHID0;
}

void PWM_set_duty_cycle(pwm_signal_t pwm_signal, uint16_t duty_cycle)
{
  if (pwm_signal == SERVO_PWM)
    REG_PWM_CDTYUPD1 |= PWM_CDTYUPD_CDTYUPD(duty_cycle);
  else if (pwm_signal == MOTOR_PWM)
    REG_PWM_CDTYUPD0 |= PWM_CDTYUPD_CDTYUPD(duty_cycle);
}
