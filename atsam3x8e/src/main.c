#include <string.h>

#include "adc.h"
#include "can.h"
#include "sam.h"
#include "servo.h"
#include "tc.h"
#include "uart.h"

#define F_CPU 84000000
#define BAUDRATE 9600

CanInit_t bit_timing = {
    .phase2 = 3,
    .propag = 3,
    .phase1 = 3,
    .sjw = 0,
    .brp = 51,
    .smp = 0,
};

CanMsg receive_can;

typedef struct
{
  int8_t x;
  int8_t y;
} joystick_pos_t;

typedef enum
{
  LEFT,
  RIGHT,
  UP,
  DOWN,
  NEUTRAL,
} joystick_dir_t;

union
{
  struct
  {
    joystick_dir_t dir : 8;
    joystick_pos_t pos;
  };
  Byte8 buffer;
} joystick_data;

int8_t servo_pos = 0;

int main()
{
  SystemInit();

  WDT->WDT_MR = WDT_MR_WDDIS; // Disable Watchdog Timer

  UART_init(F_CPU, BAUDRATE);

  CAN_init(bit_timing);

  TC_init();

  SERVO_init();
  SERVO_set_pos(servo_pos);

  ADC_init();

  printf("Setup complete\r\n");

  while (1)
  {
    while (CAN_rx(&receive_can))
      ;

    switch (receive_can.id)
    {
    case JOYSTICK_DATA_ID:
      memcpy(&joystick_data.buffer, &receive_can.byte8, sizeof(Byte8));

      servo_pos = joystick_data.pos.x;
      SERVO_set_pos(servo_pos);

      break;

    default:
      break;
    }
  }
}
