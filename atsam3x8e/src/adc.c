#include "adc.h"
#include "can.h"
#include "sam.h"

#define ADC_LOWTHRES 300
#define ADC_HIGHTHRES 400
#define MIN_REPEATED_UNDER_THRESHOLD 10

void ADC_init()
{
  REG_PMC_PCER1 |= PMC_PCER1_PID37; // Enable ADC MCK in PMC
  REG_PIOB_PDR |= PIO_PB20X1_AD13;  // Disable PIO control
  REG_PIOB_ABSR |= PIO_PB20X1_AD13; // Select A11 peripheral D

  REG_ADC_WPMR = ADC_WPMR_WPKEY(0x414443);
  REG_ADC_MR |= ADC_MR_LOWRES | ADC_MR_FREERUN | ADC_MR_PRESCAL(32);

  REG_ADC_EMR = ADC_EMR_CMPMODE_OUT | ADC_EMR_CMPSEL(13);

  REG_ADC_CWR |= ADC_CWR_LOWTHRES(ADC_LOWTHRES);
  REG_ADC_CWR |= ADC_CWR_HIGHTHRES(ADC_HIGHTHRES);

  REG_ADC_IER |= ADC_IER_COMPE;
  NVIC_EnableIRQ(ADC_IRQn);

  REG_ADC_CHER |= ADC_CHER_CH13;
}


void ADC_Handler(void)
{
  if (!(REG_ADC_ISR & ADC_ISR_COMPE))
    return;

  uint16_t adc_data = REG_ADC_LCDR;

  static bool should_end_game = true;
  static int repeated_under_thresh = 0;

  if (adc_data < ADC_LOWTHRES && should_end_game && ++repeated_under_thresh > MIN_REPEATED_UNDER_THRESHOLD)
  {
    printf("End of game\r\n");

    CanMsg goal_msg = {
      .id = GAME_END_ID,
      .length = 1,
    };

    CAN_tx(goal_msg);

    should_end_game = false;
    repeated_under_thresh = 0;
  }
  else if (adc_data > ADC_HIGHTHRES)
  {
    should_end_game = true;
  }

  NVIC_ClearPendingIRQ(ADC_IRQn);
}
