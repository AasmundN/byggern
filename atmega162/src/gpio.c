#include <avr/io.h>
#include <gpio.h>

void GPIO_init(pin_config_t *pins, int num_pins)
{
  for (uint8_t index = 0; index < num_pins; index++)
  {
    // DDRx is stored next to PORTx in memory
    volatile uint8_t *DDRx = pins[index].port - sizeof(uint8_t);

    // set pin direction
    *DDRx |= (pins[index].direction << pins[index].offset);

    // set internal pullup
    if (pins[index].direction == INPUT)
      *pins[index].port |= (pins[index].pullup << pins[index].offset);

    // set output high
    *pins[index].port |= (1 << pins[index].offset);
  }
}

void GPIO_toggle_pin(pin_config_t *pin)
{
  // toggle pin value
  *pin->port ^= (1 << pin->offset);
}
