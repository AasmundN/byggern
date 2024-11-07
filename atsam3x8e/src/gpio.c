#include "gpio.h"
#include "sam.h"
#include <stdbool.h>

void GPIO_init(pin_config_t *configs, int num_pins)
{
  for (int i = 0; i < num_pins; i++)
  {
    configs[i].pio->PIO_PER |= configs[i].pin;

    if (configs[i].direction == OUTPUT)
      configs[i].pio->PIO_OER |= configs[i].pin;
    else
      configs[i].pio->PIO_ODR |= configs[i].pin;

    if (configs[i].pullup == true)
      configs[i].pio->PIO_PUER |= configs[i].pin;
    else
      configs[i].pio->PIO_PUDR |= configs[i].pin;
  }
}

void GPIO_write(Pio *pio, uint32_t pin, bool value)
{
  if (value == true)
    pio->PIO_SODR |= pin;
  else
    pio->PIO_CODR |= pin;
}
