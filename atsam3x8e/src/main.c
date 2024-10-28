#include "can.h"
#include "sam.h"
#include "pwm.h"
#include "servo.h"
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include "tc.h"

// Import UART from Node 2 starter code, then edit include path accordingly.
// Also, remember to update the makefile
#include "uart.h"

#define F_CPU 84000000
#define BAUDRATE 9600

CanInit_t bit_timing = {
    .phase2 = 3, .propag = 3, .phase1 = 3, .sjw = 0, .brp = 51, .smp = 0};

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

  uart_init(F_CPU, BAUDRATE);
  can_init(bit_timing, 0);

  TC_init();
  SERVO_init();
  SERVO_set_pos(servo_pos);

  while (1)
  {
    while (can_rx(&receive_can));

    printf("can rx\r\n");

    switch (receive_can.id)
    {
    case JOYSTICK_DATA_ID:
      memcpy(&joystick_data.buffer, &receive_can.byte8, sizeof(Byte8));
      printf("Joystick dir: %d, Joystick pos: (%d,%d)\r\n", joystick_data.dir,
              joystick_data.pos.x, joystick_data.pos.y);

      servo_pos = joystick_data.pos.x;
      SERVO_set_pos(servo_pos);
      

      break;

    default:
      break;
    }
  }
}
