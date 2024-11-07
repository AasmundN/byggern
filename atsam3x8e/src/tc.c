#include "tc.h"
#include "sam.h"

void TC_init()
{
  // Configure TC0

  REG_PMC_PCER0 = PMC_PCER0_PID27; // Enable TC0 peripheral clock

  REG_PIOB_PDR |= PIO_PB25B_TIOA0;  // Disable PIO control
  REG_PIOB_ABSR |= PIO_PB25B_TIOA0; //

  REG_TC0_CMR0 = TC_CMR_WAVE | // Waveform mode
                 TC_CMR_WAVSEL_UP_RC
                 | // UP mode with automatic trigger on RC compare
                 TC_CMR_TCCLKS_TIMER_CLOCK3 | // Clock MCK/32
                 TC_CMR_ACPA_SET |            // RA compare effect: set
                 TC_CMR_ACPC_CLEAR;           // RC compare effect: clear

  REG_TC0_RA0 = 2625000 / 2; // Ra 0.5s
  REG_TC0_RC0 = 2625000;     // Rc 1s

  REG_TC0_CCR0 |= TC_CCR_CLKEN;
  REG_TC0_CCR0 |= TC_CCR_SWTRG;

  // Enable the RC compare interrupt
  REG_TC0_IER0 |= TC_IER_CPCS;
  NVIC_EnableIRQ(TC0_IRQn);
}

void TC0_Handler(void)
{
  printf("Pos: %d \r\n", REG_TC2_CV0);

  uint32_t status = REG_TC0_SR0;  // Read and clear status register
  NVIC_ClearPendingIRQ(TC0_IRQn); // Clear TC interrupt flag
}
