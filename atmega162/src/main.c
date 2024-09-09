#define F_CPU 4900000UL

#include <avr/interrupt.h>
#include <stdbool.h>
#include <stdio.h>
#include <util/delay.h>

#include "sys.h"
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
  SYS_init();
  UART_init(UART_BAUD(F_CPU, 9600));
  UART_rxc_register_cb(uart_rxc_cb);

  printf("\r\nSetup complete\r\n");

  while (1)
  {
    if (!should_transmit)
      continue;

    SRAM_test();

    should_transmit = false;
  }

  SRAM_test();
}
