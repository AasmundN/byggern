#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdio.h>
#include <string.h>

#include "uart.h"

// RX complete callback
void (*RX_CMPLT_cb)();

unsigned char UART_receive() { return UDR0; }

void UART_transmit(unsigned char ch)
{
  // wait for TX ready
  while (!(UCSR0A & (1 << UDRE0)))
    ;

  UDR0 = ch;
}

void default_RX_CMPLT_cb() { unsigned char ch = UART_receive(); }

void UART_init(unsigned int baudrate)
{
  cli();

  // set high and low bytes of USART0 baud rate
  UBRR1L |= (unsigned char)(baudrate >> 8);
  UBRR0L |= (unsigned char)baudrate;

  // USART0 transmit and receive enable
  UCSR0B |= (1 << TXEN0) | (1 << RXEN0);

  // USART0 set frame 8 bit and 2 stop bit
  UCSR0B |= (1 << URSEL0) | (1 << USBS0) | (3 << UCSZ00);

  // USART0 enable RX complete interrupt
  UCSR0B |= (1 << RXCIE0);

  // set USART0 RX complete interrupt callback
  RX_CMPLT_cb = default_RX_CMPLT_cb;

  sei();

  // redirect printf to UART
  fdevopen((void *)UART_transmit, NULL);
}

void UART_set_RX_CMPLT_cb(void (*cb)()) { RX_CMPLT_cb = cb; }

/**
 * Interrupt service routine for RX complete
 */
ISR(USART0_RXC_vect) { RX_CMPLT_cb(); }
