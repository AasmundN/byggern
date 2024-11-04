#include "sam.h"
#include "motor.h"

void TC2_init()
{
  REG_PMC_PCER0 = PMC_PCER0_PID29; // Enable TC2 peripheral clock

  REG_PIOC_PDR |= PIO_PC25B_TIOA6; // Disable PIO control
  REG_PIOC_PDR |= PIO_PC26B_TIOB6; // Disable PIO control

  REG_PIOC_ABSR |= PIO_PC25B_TIOA6; // Select peripheral B
  REG_PIOC_ABSR |= PIO_PC26B_TIOB6; // Select peripheral B

  REG_TC2_CMR0 |= TC_CMR_ETRGEDG_RISING | TC_CMR_ABETRG;
  REG_TC2_CMR1 |= TC_CMR_ETRGEDG_RISING | TC_CMR_ABETRG;

  REG_TC2_BMR = TC_BMR_QDEN | TC_BMR_TC0XC0S_TCLK0 | TC_BMR_POSEN;

  REG_TC2_CCR0 = TC_CCR_CLKEN;
  REG_TC2_CCR0 = TC_CCR_SWTRG;
}