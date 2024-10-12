#ifndef __CAN__
#define __CAN__

#define MAX_DATA_LENGTH 8

typedef struct
{
  int id;
  char data_length;
  char *data;
} can_msg_t;

void CAN_init();

int CAN_transmit(can_msg_t *msg);

int CAN_receive(can_msg_t *received);

#endif // __CAN__
