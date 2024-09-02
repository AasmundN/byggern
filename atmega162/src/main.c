#define F_CPU 4900000UL
#define UART_BAUD(fcpu, baud) (fcpu / 16 / baud - 1)

/*
 * External includes
 */
#include <stdbool.h>
#include <util/delay.h>

/*
 * Internal includes
 */
#include "uart.h"

volatile bool should_transmit = false;
volatile unsigned char received_ch;

void uart_rxc_cb()
{
  received_ch = UART_receive();
  should_transmit = true;
}

int main()
{
  UART_init(UART_BAUD(F_CPU, 9600));
  UART_rxc_register_cb(uart_rxc_cb);

  while (1)
  {
    if (!should_transmit)
      continue;

    UART_transmit_string("Received: ");
    UART_transmit(received_ch);
    UART_transmit_string("\r\n");

    should_transmit = false;
  }
}
