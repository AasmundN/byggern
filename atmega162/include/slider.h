#ifndef __SLIDER__
#define __SLIDER__

#include <stdint.h>

typedef enum
{
  SLIDER_LEFT,
  SLIDER_RIGHT,
} slider_t;

/**
 * Get slider postition
 * @param which slider to get
 * @return slider position mapped to 0 to 100
 */
int8_t SLIDER_get_pos(slider_t slider);

#endif // !__SLIDER__
