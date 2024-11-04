#include "tc.h"
#include "sam.h"

void TC_init()
{
  // Enable TC0 Clock in the PMC enable register PMC_PCER0
  PMC->PMC_PCER0 |= PMC_PCER0_PID27;

  PIOB->PIO_PDR |= PIO_PB25B_TIOA0;  // Disable PIO control
  PIOB->PIO_ABSR |= PIO_PB25B_TIOA0; // Select peripheral B

  // Configure the TC channel mode register TC_CMR0:
  TC0->TC_CHANNEL[0].TC_CMR = TC_CMR_WAVE |                // Waveform mode
                              TC_CMR_WAVSEL_UP_RC |        // UP mode with automatic trigger on RC compare
                              TC_CMR_TCCLKS_TIMER_CLOCK3 | // Clock MCK/32
                              TC_CMR_ACPA_SET |            // RA compare effect: set
                              TC_CMR_ACPC_CLEAR;           // RC compare effect: clear

  // Configure the TC registers A,B,C
  TC0->TC_CHANNEL[0].TC_RA = 2625000/2; // Ra 0.5s
  TC0->TC_CHANNEL[0].TC_RC = 2625000; // Rc 1s

  // Set CLKEN bit in the TC_CCRx register to enable the clock
  TC0->TC_CHANNEL[0].TC_CCR = TC_CCR_CLKEN;
  TC0->TC_CHANNEL[0].TC_CCR = TC_CCR_SWTRG; // Start the timer

  // Enable the RC compare interrupt
  TC0->TC_CHANNEL[0].TC_IER |= TC_IER_CPCS;
  NVIC_EnableIRQ(TC0_IRQn);
}

void TC0_Handler(void)
{
  // clear status register by reading
  uint32_t status = TC0->TC_CHANNEL[0].TC_SR;

  NVIC_ClearPendingIRQ(TC0_IRQn);
}
