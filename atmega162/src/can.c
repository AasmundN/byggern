#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdio.h>

#include "MCP2515.h"
#include "can.h"

#define TXB0CNTRL 0x3f
#define REQOP_bm 0b111
#define LOOPBACK_MODE 2

ISR(INT0_vect) { printf("INT0\r\n"); }

void CAN_init()
{
  MCP2515_init();

  // set loopback mode for transmit buffer 0
  MCP2515_bit_mod(TXB0CNTRL, REQOP_bm, LOOPBACK_MODE);

  cli();

  // INT0 pin as input
  DDRD &= ~(1 << PD0);

  // INT0 pin internal pull-up
  PORTD |= (1 << PD0);

  // INT0 falling edge
  MCUCR |= 0b10;

  // INT0 external interrupt enable
  GICR |= (1 << INT0);

  sei();
}

void CAN_transmit(can_msg_t *msg) {}
