#define F_CPU 4900000UL

#include <avr/interrupt.h>
#include <stdbool.h>
#include <stdio.h>
#include <util/delay.h>

#include "adc.h"
#include "can.h"
#include "game_menu.h"
#include "gpio.h"
#include "oled.h"
#include "sys.h"
#include "timer.h"
#include "uart.h"

#define NUM_PINS 1

typedef enum
{
  HEARTBEAT_LED
} pin_t;

// configure GPIO pins
pin_config_t pins[NUM_PINS] = {

    // HEARTBEAT_LED
    {
        .offset = PB2,
        .port = &PORTB,
        .direction = OUTPUT,
        .pullup = false,
    } // end HEARTBEAT_LED
};

volatile unsigned char received_ch;

void uart_receive_cb()
{
  received_ch = UART_receive();
  printf("Received: %c \r\n", received_ch);
}

void heartbeat_cb() { GPIO_toggle_pin(&pins[HEARTBEAT_LED]); }

int main()
{
  SYS_init();

  GPIO_init(pins, NUM_PINS);

  UART_init(UART_BAUD(F_CPU, 9600));
  UART_set_RX_CMPLT_cb(uart_receive_cb);

  TIMER_init();
  TIMER_set_TIMER1_COMPA_cb(heartbeat_cb);

  GAME_MENU_init();

  OLED_init();
  OLED_clear_screen();
  OLED_refresh();

  ADC_calibrate_joystick();

  CAN_init();

  printf("\r\nSetup complete\r\n");

  OLED_clear_screen();
  OLED_print("Setup complete", 0, 0);
  OLED_refresh();

  union
  {
    struct
    {
      joystick_dir_t dir : 8;
      joystick_pos_t pos;
    };
    uint8_t buffer[3];
  } joystick_data;

  can_msg_t joystick_can_msg = {
      .id = JOYSITCK_ID, .data = joystick_data.buffer, .data_length = 3};

  while (1)
  {
    joystick_pos_t pos = ADC_get_joystick_pos();

    joystick_data.dir = ADC_calc_joystick_dir(pos);
    joystick_data.pos = pos;

    CAN_transmit(&joystick_can_msg);

    _delay_ms(200);
  }
}
