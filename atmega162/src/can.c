#pragma clang diagnostic ignored "-Wbitfield-constant-conversion"

#include <avr/interrupt.h>
#include <avr/io.h>

#include "can.h"
#include "mcp2515.h"
#include "mcp2515_defines.h"

#define SIDH_offset 0x1
#define SIDL_offset 0x2
#define DLC_offset 0x5
#define DATA_offset 0x6

#define REQOP_bm 0b11100000
#define TXB0_TXREQ_bm 0b100
#define DLC_bm 0b1111

volatile int data_pending = 0;

ISR(INT0_vect) { data_pending = 1; }

void CAN_init()
{
  MCP2515_init();

  uint8_t BRP = 4;
  uint8_t BTL = 1;
  uint8_t PRSEG = 3;
  uint8_t PHSEG1 = 3;
  uint8_t PHSEG2 = 3;

  uint8_t CNF1 = BRP;
  MCP2515_write(MCP_CNF1, &CNF1, 1);

  uint8_t CNF2 = (BTL << 7) | (PHSEG1 << 3) | (PRSEG);
  MCP2515_write(MCP_CNF2, &CNF2, 1);

  uint8_t CNF3 = PHSEG2;
  MCP2515_write(MCP_CNF3, &CNF3, 1);

  // set normal mode for TXB0
  MCP2515_bit_mod(MCP_TXB0CTRL + MCP_CANCTRL, REQOP_bm, MODE_NORMAL);

  // enable interrupts on RXB0 and RXB1
  MCP2515_bit_mod(MCP_CANINTE, MCP_RX_INT, MCP_RX_INT);

  // disable all transmit interrupts
  MCP2515_bit_mod(MCP_CANINTE, MCP_TX0IF | MCP_TX1IF | MCP_TX2IF, MCP_NO_INT);

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
  cli();

  // check transmission in progress or invalid data length
  uint8_t status = MCP2515_read_status();
  if (status & TXB0_TXREQ_bm | msg->data_length > MAX_DATA_LENGTH)
    return 1;

  uint8_t SIDH = msg->id >> 3;
  uint8_t SIDL = msg->id << 5;
  uint8_t DLC = msg->data_length;

  // write ID and data length
  MCP2515_write(MCP_TXB0CTRL + SIDH_offset, &SIDH, 1);
  MCP2515_write(MCP_TXB0CTRL + SIDL_offset, &SIDL, 1);
  MCP2515_write(MCP_TXB0CTRL + DLC_offset, &DLC, 1);

  // write data
  MCP2515_write(MCP_TXB0CTRL + DATA_offset, msg->data, msg->data_length);

  MCP2515_rts(MCP_RTS_TX0);

  sei();

  return 0;
}

int CAN_receive(can_msg_t *received)
{
  if (!data_pending)
    return 1;

  cli();

  uint8_t status = MCP2515_read_status();
  uint8_t buffer_addr;

  if (status & MCP_RX0IF)
    buffer_addr = MCP_RXB0CTRL;
  else if (status & MCP_RX1IF)
    buffer_addr = MCP_RXB1CTRL;

  uint8_t SIDH, SIDL, DLC;

  // write ID and data length
  MCP2515_read(buffer_addr + SIDH_offset, &SIDH, 1);
  MCP2515_read(buffer_addr + SIDL_offset, &SIDL, 1);
  MCP2515_read(buffer_addr + DLC_offset, &DLC, 1);

  received->id = ((int)SIDH << 3) | ((SIDL & 0b11100000) >> 5);
  received->data_length = DLC & DLC_bm;

  // write data
  MCP2515_read(buffer_addr + DATA_offset, received->data,
               received->data_length);

  // reset interrupt flag
  MCP2515_bit_mod(MCP_CANINTF, status & MCP_RX_INT, 0);

  data_pending = 0;

  sei();

  return 0;
}
