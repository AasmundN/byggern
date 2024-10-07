#ifndef __CAN__
#define __CAN__

#define MAX_DATA_LENGTH 8

typedef struct
{
  int id;
  char data[MAX_DATA_LENGTH];
  int data_length;
} can_msg_t;

void CAN_init();

void CAN_transmit(can_msg_t *msg);

#endif // __CAN__
