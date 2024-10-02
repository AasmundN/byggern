#ifndef __CAN__
#define __CAN__

void CAN_init();

void CAN_controller_read(char addr, char data_buffer[], int buffer_size);

void CAN_controller_write(char addr, char data_buffer[], int buffer_size);

#endif // __CAN__
