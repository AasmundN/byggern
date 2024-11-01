#include "adc.h"
#include "sam.h"

void ADC_init()
{
  REG_PMC_PCER1 |= PMC_PCER1_PID37; // Enable ADC MCK in PMC
  REG_PIOB_PDR |= PIO_PB20X1_AD13;  // Disable PIO control
  REG_PIOB_ABSR |= PIO_PB20X1_AD13; // Select A11 peripheral D

  REG_ADC_WPMR = ADC_WPMR_WPKEY(0x414443);
  REG_ADC_MR |= ADC_MR_LOWRES | ADC_MR_FREERUN | ADC_MR_PRESCAL(2);

  REG_ADC_EMR = ADC_EMR_CMPMODE_LOW | ADC_EMR_CMPSEL(13);

  REG_ADC_CWR |= ADC_CWR_LOWTHRES(600);

  REG_ADC_IER |= ADC_IER_COMPE;
  NVIC_EnableIRQ(ADC_IRQn);

  REG_ADC_CHER |= ADC_CHER_CH13;
}

void ADC_Handler(void)
{
  if (!(REG_ADC_ISR & ADC_ISR_COMPE))
    return;

  uint8_t CMPMODE = REG_ADC_EMR & ADC_EMR_CMPMODE_Msk;

  if (CMPMODE == ADC_EMR_CMPMODE_LOW)
  {
    printf("GOAL at: %d\r\n", REG_ADC_LCDR);
    REG_ADC_EMR = ADC_EMR_CMPMODE_HIGH;
  }
  else if (CMPMODE == ADC_EMR_CMPMODE_HIGH)
  {
    REG_ADC_EMR = ADC_EMR_CMPMODE_LOW;
  }

  NVIC_ClearPendingIRQ(ADC_IRQn);
}
