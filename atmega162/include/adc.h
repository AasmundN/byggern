#ifndef __ADC__
#define __ADC__

#include <stdint.h>

#define NUM_ADC_CHANNELS 4

typedef enum
{
  JOYSTICK_X_INDEX,
  JOYSTICK_Y_INDEX,
  SLIDER_RIGHT_INDEX,
  SLIDER_LEFT_INDEX,
} adc_result_index_t;

/**
 * Read all ADC channels
 * @param result read result is placed here
 */
void ADC_read(uint8_t *result);

#endif // __ADC__
