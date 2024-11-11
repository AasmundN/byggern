#ifndef __ENCODER__
#define __ENCODER__

#include <stdint.h>

/**
 * Initialize TC2 in quadrature decoder mode
 */
void ENCODER_init();

/**
 * Read encoder position
 * @returns encoder position
 */
int16_t ENCODER_read();

#endif // __ENCODER__
