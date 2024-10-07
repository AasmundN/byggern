#ifndef __CAN__
#define __CAN__

#define T0 1
#define T1 2
#define T2 4

void CAN_init();

void CAN_controller_read(char addr, char data_buffer[], int buffer_size);

void CAN_controller_write(char addr, char data_buffer[], int buffer_size);

void CAN_controller_rts(int buffer_select_bits);

char CAN_controller_read_status();

void CAN_controller_bit_mod(char addr, char mask, char data);

void CAN_controller_reset();

#endif // __CAN__
