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

  char tx_buffer[MAX_DATA_LENGTH] = "Amobius";
  char rx_buffer[MAX_DATA_LENGTH];

  can_msg_t msg = {.id = 9, .data = tx_buffer, .data_length = MAX_DATA_LENGTH};
  can_msg_t received_msg = {.data = rx_buffer};

  while (1)
  {
    while (CAN_transmit(&msg))
      ;

    while (CAN_receive(&received_msg))
      ;

    for (int i = 0; i < MAX_DATA_LENGTH; i++)
      printf("%c", received_msg.data[i]);
    printf("\r\n");

    _delay_ms(250);
  }
}
