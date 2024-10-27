#include "tc.h"
#include "sam.h"

void PWM_tc_init()
{
  // Enable TC Clock in the PMC enable register PMC_PCER0
  PMC->PMC_PCER0 |= (1 << ID_TC0);

  // Configure PIO controller to control the TC channel 0 pin
  PIOA->PIO_PDR = PIO_PB25B_TIOA0;    // Disable PIO control
  PIOA->PIO_ABSR |= PIO_PB25B_TIOA0;  // Enable peripheral control

  // Configure the TC channel mode register TC_CMR0:
  TC0->TC_CHANNEL[0].TC_CMR = TC_CMR_WAVE | // Waveform mode
                              TC_CMR_WAVSEL_UP_RC | // UP mode with automatic trigger on RC compare
                              TC_CMR_TCCLKS_TIMER_CLOCK3 | // Clock MCK/32
                              TC_CMR_ACPA_SET |  // RA compare effect: set
                              TC_CMR_ACPC_CLEAR; // RC compare effect: clear

  // Configure the TC registers A,B,C
  TC0->TC_CHANNEL[0].TC_RA = 3938; // Ra is the wave midpoint = F_CPU/32*1.5ms
  TC0->TC_CHANNEL[0].TC_RC = 52500; // Rc resets the wave = F_CPU/32*20ms

  // Set CLKEN bit in the TC_CCRx register to enable the clock
  TC0->TC_CHANNEL[0].TC_CCR = TC_CCR_CLKEN;

  // Enable the RC compare interrupt
  TC0->TC_CHANNEL[0].TC_IER = TC_IER_CPCS;
  NVIC_EnableIRQ(TC0_IRQn);
}

void PWM_tc_start()
{
  TC0->TC_CHANNEL[0].TC_CCR = TC_CCR_SWTRG; // Start the timer
}

void PWM_tc_stop()
{
  // Ensure the timer is running before attempting to stop it
  if (TC0->TC_CHANNEL[0].TC_SR & TC_SR_CLKSTA)
  {
    TC0->TC_CHANNEL[0].TC_CCR = TC_CCR_CLKDIS; // Disable the clock
    NVIC_DisableIRQ(TC0_IRQn);                 // Disable TC0 interrupt
    TC0->TC_CHANNEL[0].TC_IDR = TC_IDR_CPCS;   // Disable RC compare interrupt
    TC0->TC_CHANNEL[0].TC_SR;                  // Clear any pending interrupt flags
  }
}

void TC0_Handler(void)
{
    // Clear the interrupt by reading the TC_SR register
    uint32_t status = TC0->TC_CHANNEL[0].TC_SR;
    if (status & TC_SR_CPCS) {
      // Implement stuff here later
    }
}
