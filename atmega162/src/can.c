#include <avr/interrupt.h>
#include <avr/io.h>

#include "MCP2515.h"
#include "can.h"

#define CANINTE 0x2b
#define CANINTF 0x2c

#define TXB0CNTRL 0x3f

#define REQOP_bm 0b11100000
#define LOOPBACK_MODE 2

#define TXB0_TXREQ 0b100
#define TXB0_ADDR 0x30
#define RXB0_ADDR 0x60

int data_received = 0;

void (*receive_cb)(can_msg_t *msg);

void default_receive_cb(can_msg_t *msg) {}

void CAN_set_receive_cb(void (*cb)(can_msg_t *)) { receive_cb = cb; }

ISR(INT0_vect)
{
  cli();

  data_received = 1;

  sei();
}

void CAN_init()
{
  MCP2515_init();

  // set loopback mode for transmit buffer 0
  MCP2515_bit_mod(TXB0CNTRL, REQOP_bm, LOOPBACK_MODE << 5);

  // enable interrupts on both RX buffers
  MCP2515_bit_mod(CANINTE, 0b11, 0b11);

  cli();

  // INT0 pin as input
  DDRD &= ~(1 << PD0);

  // INT0 pin internal pull-up
  PORTD |= (1 << PD0);

  // INT0 falling edge
  MCUCR |= 0b10;

  // INT0 external interrupt enable
  GICR |= (1 << INT0);

  receive_cb = default_receive_cb;

  sei();
}

int CAN_transmit(can_msg_t *msg)
{
  cli();

  char status = MCP2515_read_status();
  if (status & TXB0_TXREQ)
    return 1;

  char TXB0SIDH_val = msg->id >> 3;
  char TXB0SIDL_val = msg->id << 5;
  char TXB0DLC_val = msg->data_length;

  MCP2515_write(TXB0_ADDR + 1, &TXB0SIDH_val, 1);
  MCP2515_write(TXB0_ADDR + 2, &TXB0SIDL_val, 1);
  MCP2515_write(TXB0_ADDR + 5, &TXB0DLC_val, 1);

  MCP2515_write(TXB0_ADDR + 6, msg->data, msg->data_length);

  MCP2515_rts(T0);

  sei();

  return 0;
}

void CAN_receive()
{
  if (!data_received)
    return;

  // read out data and make can_msg_t
  can_msg_t msg;

  char TXB0SIDH_val;
  char TXB0SIDL_val;
  char TXB0DLC_val;

  MCP2515_read(RXB0_ADDR + 1, &TXB0SIDH_val, 1);
  MCP2515_read(RXB0_ADDR + 2, &TXB0SIDL_val, 1);
  MCP2515_read(RXB0_ADDR + 5, &TXB0DLC_val, 1);

  msg.id = ((int)TXB0SIDH_val << 3) | (TXB0SIDL_val >> 5);
  msg.data_length = TXB0DLC_val & 0b1111;
  MCP2515_read(RXB0_ADDR + 6, msg.data, msg.data_length);

  // receive callback
  receive_cb(&msg);

  // clear interrupt flag
  MCP2515_bit_mod(CANINTF, 0b1, 0);
}
