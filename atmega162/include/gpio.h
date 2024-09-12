#ifndef __GPIO__
#define __GPIO__

#include <avr/io.h>
#include <stdbool.h>
#include <stdint.h>

typedef enum
{
  INPUT,
  OUTPUT,
} pin_direction_t;

typedef struct
{
  uint8_t offset;
  volatile uint8_t *port;
  pin_direction_t direction;
  bool pullup;
} pin_config_t;

/**
 * Configure GPIO pins
 * @param pins array of pin config structs
 * @param num_pins number of pins in array
 */
void GPIO_init(pin_config_t *pins, int num_pins);

/**
 * Toggle pin state
 * @param pin pointer to pin config
 */
void GPIO_toggle_pin(pin_config_t *pin);

#endif // __GPIO__
