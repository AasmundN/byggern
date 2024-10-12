#define F_CPU 4900000UL

#include <avr/interrupt.h>
#include <stdbool.h>
#include <stdio.h>
#include <util/delay.h>

#include "adc.h"
#include "game_menu.h"
#include "gpio.h"
#include "mcp2515.h"
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

  MCP2515_init();

  printf("\r\nSetup complete\r\n");

  OLED_clear_screen();
  OLED_print("Setup complete", 0, 0);
  OLED_refresh();

  while (1)
  {
    char data = 'a';
    MCP2515_write(0x37, &data, 1);

    char rx;
    MCP2515_read(0x37, &rx, 1);

    printf("%c\r\n", rx);
    _delay_ms(250);
  }
}
