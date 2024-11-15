#include <avr/io.h>
#include <util/delay.h>

#include "adc.h"

#define ADC_REG (*(uint8_t *)0x1400)
#define T_CONV 32 // microseconds

void ADC_read(uint8_t *result)
{
  // Initiate ADC conversion
  ADC_REG = '\0';

  // Wait for conversion to complete
  _delay_us(T_CONV);

  // Read all ADC channels
  for (uint8_t i = 0; i < NUM_ADC_CHANNELS; i++)
    result[i] = ADC_REG;
}
