/*
 * External includes
 */
#include <avr/interrupt.h>
#include <avr/io.h>
#include <string.h>

/*
 * Internal includes
 */
#include "uart.h"

void (*rx_cb)();

unsigned char UART_receive() { return UDR0; }

void UART_transmit(unsigned char ch)
{
  while (!(UCSR0A & (1 << UDRE0)))
    ;

  UDR0 = ch;
}

void default_rxc_cb() { unsigned char ch = UART_receive(); }

void UART_init(unsigned int baudrate)
{
  cli();

  // set high and low bytes of USART0 baud rate
  UBRR1L |= (unsigned char)(baudrate >> 8);
  UBRR0L |= (unsigned char)baudrate;

  // USART0 transmit and receive enable
  UCSR0B |= (1 << TXEN0) | (1 << RXEN0);

  // USART 0 set frame 8 bit and 2 stop bit
  UCSR0B |= (1 << URSEL0) | (1 << USBS0) | (3 << UCSZ00);

  // USART0 enable RX complete interrupt
  UCSR0B |= (1 << RXCIE0);

  // set USART0 RX complete interrupt callback
  rx_cb = default_rxc_cb;

  sei();
}

void UART_transmit_string(char *string) {
  for (size_t i = 0; i < strlen(string); i++)
    UART_transmit(string[i]);
}

void UART_rxc_register_cb(void (*cb)()) { rx_cb = cb; }

ISR(USART0_RXC_vect) { rx_cb(); }
