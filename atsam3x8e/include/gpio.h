#ifndef __GPIO__
#define __GPIO__

#include "sam.h"

typedef enum
{
  INPUT,
  OUTPUT,
} pin_direction_t;

typedef struct
{
  volatile Pio *pio;
  uint32_t pin;
  pin_direction_t direction;
  bool pullup;
} pin_config_t;

void GPIO_init(pin_config_t *configs, int num_pins);

void GPIO_write(Pio *pio, uint32_t pin, bool value);

#endif // __GPIO__
