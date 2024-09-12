#define F_CPU 4900000UL

#include <avr/interrupt.h>
#include <stdbool.h>
#include <stdio.h>
#include <util/delay.h>

#include "gpio.h"
#include "sys.h"
#include "uart.h"

#define NUM_PINS 1

typedef enum
{
  HEARTBEAT_LED
} pin_t;

// configure GPIO pins
pin_config_t pins[NUM_PINS] = {

    // HEARTBEAT_LED
    {
        PB5,    // pin offset
        &PORTB, // port
        OUTPUT, // pin direction
        false,  // internal pullup
    } // end HEARTBEAT_LED
};

volatile bool should_transmit = false;
volatile unsigned char received_ch;

void uart_rxc_cb()
{
  received_ch = UART_receive();
  should_transmit = true;
}

int main()
{
  SYS_init();
  UART_init(UART_BAUD(F_CPU, 9600));
  UART_rxc_register_cb(uart_rxc_cb);
  GPIO_init(pins, NUM_PINS);

  printf("\r\nSetup complete\r\n");

  while (1)
  {
    if (!should_transmit)
      continue;

    GPIO_toggle_pin(&pins[HEARTBEAT_LED]);

    should_transmit = false;
  }
}
