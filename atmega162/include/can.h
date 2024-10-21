#ifndef __CAN__
#define __CAN__

#include <stdint.h>

#define MAX_DATA_LENGTH 8

typedef struct
{
  uint16_t id;
  uint8_t data_length;
  uint8_t *data; // remember to allocate memory for this array
} can_msg_t;

typedef enum
{
  JOYSITCK_ID = 42
} can_id_t;

void CAN_init();

/**
 * Attempt to transmit msg on CAN bus
 * @param msg to send
 * @return 0 for success, 1 for failure
 */
int CAN_transmit(can_msg_t *msg);

/**
 * Receive CAN msg, should be polled regularely
 * @param received msg destination
 * @return 0 for success, 1 for failure
 */
int CAN_receive(can_msg_t *received);

#endif // __CAN__
