#define F_CPU 4900000UL

#include <avr/interrupt.h>
#include <stdbool.h>
#include <stdint.h>
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

#define NUM_PINS 3

typedef enum
{
  HEARTBEAT_LED,
  JOYSTICK_BTN,
  TOUCH_BTN_LEFT
} pin_t;

// configure GPIO pins
pin_config_t pins[NUM_PINS] = {

    // HEARTBEAT_LED
    {
        .offset = PB2,
        .port = &PORTB,
        .direction = OUTPUT,
        .pullup = false,
    }, // end HEARTBEAT_LED

    // JOYSTICK_BTN
    {
        .offset = PE0,
        .port = &PORTE,
        .direction = INPUT,
        .pullup = true,
    }, // end JOYSTICK_BTN

    // TOUCH_BTN_LEFT
    {
        .offset = PE2,
        .port = &PORTE,
        .direction = INPUT,
        .pullup = false,
    } // end TOUCH_BTN_LEFT
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
      // joystick
      joystick_dir_t joystick_dir : 8;
      joystick_pos_t joystick_pos;
      uint8_t joystick_btn_state;

      // slider
      int8_t slider_pos;
      uint8_t slider_btn_state;
    } __attribute__((packed));
    uint8_t buffer[6];
  } input_data;

  can_msg_t input_data_can_msg = {
      .id = INPUT_DATA_ID,
      .data = input_data.buffer,
      .data_length = sizeof(input_data.buffer),
  };

  while (1)
  {
    joystick_pos_t pos = ADC_get_joystick_pos();

    input_data.joystick_dir = ADC_calc_joystick_dir(pos);
    input_data.joystick_pos = pos;
    input_data.joystick_btn_state = GPIO_read_pin(&pins[JOYSTICK_BTN]);

    input_data.slider_pos = ADC_get_slider_pos(SLIDER_LEFT_INDEX);
    input_data.slider_btn_state = GPIO_read_pin(&pins[TOUCH_BTN_LEFT]);

    CAN_transmit(&input_data_can_msg);

    _delay_ms(20);
  }
}
