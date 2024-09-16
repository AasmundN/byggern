#include <stdint.h>
#ifndef __ADC__
#define __ADC__

#define NUM_ADC_CHANNELS 4

void ADC_read(uint8_t *result);

#endif // __ADC__