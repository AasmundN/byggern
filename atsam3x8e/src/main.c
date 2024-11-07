#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "adc.h"
#include "can.h"
#include "encoder.h"
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
    // joystick
    joystick_dir_t joystick_dir : 8;
    joystick_pos_t joystick_pos;
    uint8_t joystick_btn_state;

    // slider
    int8_t slider_pos;
    uint8_t slider_btn_state;
  } __attribute__((packed));
  Byte8 buffer;
} input_data;

int8_t servo_pos = 0;

void timer_interrupt_cb() { printf("Pos: %d \r\n", ENCODER_read()); }

int main()
{
  SystemInit();

  WDT->WDT_MR = WDT_MR_WDDIS; // Disable Watchdog Timer

  UART_init(F_CPU, BAUDRATE);

  CAN_init(bit_timing);

  ENCODER_init();

  TC_init(F_CPU / 1000); // period of 1s
  TC_set_cb(timer_interrupt_cb);

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
    case INPUT_DATA_ID:
      memcpy(&input_data.buffer, &receive_can.byte8, sizeof(Byte8));

      servo_pos = input_data.joystick_pos.x;
      SERVO_set_pos(servo_pos);

      break;

    default:
      break;
    }
  }
}
