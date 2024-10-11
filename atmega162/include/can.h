#ifndef __CAN__
#define __CAN__

typedef struct
{
  int id;
  char *data;
  int data_length;
} can_msg_t;

/**
 * Initialize CAN driver
 */
void CAN_init();

/**
 * Transmit a CAN message
 * @param msg to transmit
 * @return 0 for success, 1 for failure
 */
int CAN_transmit(can_msg_t *msg);

/**
 * Receive a CAN message, should be polled regularely
 * @param msg receive destination
 * @return 0 for success, 1 for failure
 */
int CAN_receive(can_msg_t *received);

#endif // __CAN__
