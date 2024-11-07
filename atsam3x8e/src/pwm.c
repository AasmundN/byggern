#include "pwm.h"
#include "sam.h"

void PWM_init(pwm_signal_t pwm_signal, uint16_t period)
{
  if (pwm_signal == SERVO_PWM)
  {

    REG_PMC_PCER1 |= PMC_PCER1_PID36; // Enable PWM in PMC enable register.
    REG_PIOB_PDR |= PIO_PB13B_PWMH1;  // Disable PIO control
    REG_PIOB_ABSR |= PIO_PB13B_PWMH1; // Select PWMH1 peripheral B

    REG_PWM_WPCR = PWM_WPCR_WPKEY(0x50574D) | PWM_WPCR_WPCMD(0) | // Unlock UI
                   PWM_WPCR_WPRG0 | PWM_WPCR_WPRG1 | PWM_WPCR_WPRG2
                   | PWM_WPCR_WPRG3 | PWM_WPCR_WPRG4 | PWM_WPCR_WPRG5;

    REG_PWM_CMR1 |= PWM_CMR_CPRE_MCK_DIV_32 | PWM_CMR_CPOL; // MCK/32 clock
    REG_PWM_CPRD1 |= PWM_CPRD_CPRD(period);
    REG_PWM_CDTY1 |= PWM_CDTY_CDTY(0);
  }
}

void PWM_stop(pwm_signal_t pwm_signal)
{
  if (pwm_signal == SERVO_PWM)
    REG_PWM_DIS |= PWM_DIS_CHID1;
}

void PWM_start(pwm_signal_t pwm_signal)
{
  if (pwm_signal == SERVO_PWM)
    REG_PWM_ENA |= PWM_ENA_CHID1;
}

void PWM_set_duty_cycle(pwm_signal_t pwm_signal, uint16_t duty_cycle)
{
  if (pwm_signal == SERVO_PWM)
    REG_PWM_CDTYUPD1 |= PWM_CDTYUPD_CDTYUPD(duty_cycle);
}
