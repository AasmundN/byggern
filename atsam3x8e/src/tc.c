#include "tc.h"
#include "sam.h"

void (*tc_cb)();

void tc_cb_default() {};

void TC_init(unsigned long period)
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

  REG_TC0_RA0 = period / 2;
  REG_TC0_RC0 = period;

  // set timer interrupt callback
  tc_cb = tc_cb_default;

  REG_TC0_CCR0 |= TC_CCR_CLKEN;
  REG_TC0_CCR0 |= TC_CCR_SWTRG;

  // Enable the RC compare interrupt
  REG_TC0_IER0 |= TC_IER_CPCS;
}

void TC_set_cb(void (*cb)()) { tc_cb = cb; }

void TC_enable() { NVIC_EnableIRQ(TC0_IRQn); }

void TC_disable() { NVIC_DisableIRQ(TC0_IRQn); }

void TC0_Handler(void)
{
  tc_cb();

  uint32_t status = REG_TC0_SR0;  // Read and clear status register
  NVIC_ClearPendingIRQ(TC0_IRQn); // Clear TC interrupt flag
}
