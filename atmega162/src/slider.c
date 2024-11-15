#include "slider.h"
#include "adc.h"

uint8_t adc_buffer[NUM_ADC_CHANNELS];

int8_t SLIDER_get_pos(slider_t slider)
{
  ADC_read(adc_buffer);

  return slider == SLIDER_RIGHT ? adc_buffer[SLIDER_RIGHT_INDEX] * 100 / 255
                                : adc_buffer[SLIDER_LEFT_INDEX] * 100 / 255;
}
