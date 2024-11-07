#include "encoder.h"
#include "sam.h"

void ENCODER_init()
{
  // Configure TC2 in quardature mode

  REG_PMC_PCER1 = PMC_PCER1_PID33; // Enable TC2 peripheral clock

  REG_PIOC_PDR |= PIO_PC25B_TIOA6; // Disable PIO control
  REG_PIOC_PDR |= PIO_PC26B_TIOB6; // Disable PIO control

  REG_PIOC_ABSR |= PIO_PC25B_TIOA6; // Select peripheral B
  REG_PIOC_ABSR |= PIO_PC26B_TIOB6; // Select peripheral B

  REG_TC2_CMR0 |= TC_CMR_ETRGEDG_RISING | TC_CMR_ABETRG | TC_CMR_TCCLKS_XC0;

  REG_TC2_BMR |= TC_BMR_QDEN | TC_BMR_POSEN | TC_BMR_EDGPHA;

  REG_TC2_CCR0 |= TC_CCR_CLKEN;

  REG_TC2_BCR |= TC_BCR_SYNC;
}

int16_t ENCODER_read() { return REG_TC2_CV0; }
