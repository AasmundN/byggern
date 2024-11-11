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


/**
 * Initialize GPIO pins
 * @param configs struct of pin configurations
 * @param num_pins number of pins to initialize
 */
void GPIO_init(pin_config_t *configs, int num_pins);

/**
 * Write a value to a GPIO pin
 * @param pio pointer to the PIO controller
 * @param pin pin number
 * @param value value to write (true or false)
 */
void GPIO_write(Pio *pio, uint32_t pin, bool value);

#endif // __GPIO__
