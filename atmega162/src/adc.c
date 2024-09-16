#ifndef F_CPU
#define F_CPU 4900000UL
#endif // F_CPU
#define ADC_START_ADDR 0x1400
#define T_CONV 15 // microseconds

#include <util/delay.h>
#include <stdio.h>

#include "adc.h"

void ADC_read(uint8_t *result)
{
  volatile uint8_t *adc = (uint8_t *)ADC_START_ADDR;
  // Initiate ADC conversion
  *adc = '\0';
  // Wait for conversion to complete
  _delay_us(T_CONV);
  // Read all ADC channels
  for(uint8_t i=0;i<NUM_ADC_CHANNELS;i++)
  {
    result[i] = *adc;
  }
}