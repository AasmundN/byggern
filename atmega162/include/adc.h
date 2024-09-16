#include <stdint.h>

#ifndef __ADC__
#define __ADC__

#define NUM_ADC_CHANNELS 4

/**
 * Read all ADC channels
 * @param result read result is placed here
 */
void ADC_read(uint8_t *result);

#endif // __ADC__
