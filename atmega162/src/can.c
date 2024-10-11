#include <avr/interrupt.h>
#include <avr/io.h>

#include "can.h"
#include "mcp2515.h"
#include "mcp2515_defines.h"

#define TXB0_TXREQ_bm 0b100
#define REQOP_bm 0b11100000
#define DLC_bm 0b1111

#define SIDH_REG_offset 0x1
#define SIDL_REG_offset 0x2
#define DLC_REG_offset 0x5
#define DATA_REG_offset 0x6

int data_received = 0;

ISR(INT0_vect) { data_received = 1; }

void CAN_init()
{
  MCP2515_init();

  // set loopback mode for transmit buffer 0
  MCP2515_bit_mod(MCP_TXB0CTRL + MCP_CANCTRL, REQOP_bm, MODE_LOOPBACK);

  // enable interrupts on both RX buffers
  MCP2515_bit_mod(MCP_CANINTE, MCP_RX0IF | MCP_RX1IF, MCP_RX0IF | MCP_RX1IF);

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

int CAN_transmit(can_msg_t *msg)
{
  char status = MCP2515_read_status();
  if (status & TXB0_TXREQ_bm)
    return 1;

  char TXB0SIDH = msg->id >> 3;
  char TXB0SIDL = msg->id << 5;
  char TXB0DLC = msg->data_length;

  MCP2515_write(MCP_TXB0CTRL + SIDH_REG_offset, &TXB0SIDH, 1);
  MCP2515_write(MCP_TXB0CTRL + SIDL_REG_offset, &TXB0SIDL, 1);
  MCP2515_write(MCP_TXB0CTRL + DLC_REG_offset, &TXB0DLC, 1);

  MCP2515_write(MCP_TXB0CTRL + DATA_REG_offset, msg->data, msg->data_length);

  MCP2515_rts(MCP_RTS_TX0);

  return 0;
}

int CAN_receive(can_msg_t *received)
{
  if (!data_received)
    return 1;

  char CANINTF;
  MCP2515_read(MCP_CANINTF, &CANINTF, 1);

  char buffer_addr;

  if (CANINTF & MCP_RX0IF)
    buffer_addr = MCP_RXB0CTRL;
  else if (CANINTF & MCP_RX1IF)
    buffer_addr = MCP_RXB1CTRL;

  char TXB0SIDH, TXB0SIDL, TXB0DLC;

  MCP2515_read(buffer_addr + SIDH_REG_offset, &TXB0SIDH, 1);
  MCP2515_read(buffer_addr + SIDL_REG_offset, &TXB0SIDL, 1);
  MCP2515_read(buffer_addr + DLC_REG_offset, &TXB0DLC, 1);

  received->id = ((int)TXB0SIDH << 3) | (TXB0SIDL >> 5);
  received->data_length = TXB0DLC & DLC_bm;

  MCP2515_read(buffer_addr + DATA_REG_offset, received->data,
               received->data_length);

  // clear interrupt flag
  MCP2515_bit_mod(MCP_CANINTF, CANINTF & (MCP_RX0IF | MCP_RX1IF), 0);

  return 0;
}
