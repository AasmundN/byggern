#ifndef __CAN__
#define __CAN__

typedef struct
{
  int id;
  char *data;
  int data_length;
} can_msg_t;

void CAN_init();

void CAN_set_receive_cb(void (*cb)(can_msg_t *));

int CAN_transmit(can_msg_t *msg);

void CAN_receive();

#endif // __CAN__
