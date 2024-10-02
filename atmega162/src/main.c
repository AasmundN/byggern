#define F_CPU 4900000UL

#include <avr/interrupt.h>
#include <stdbool.h>
#include <stdio.h>
#include <util/delay.h>

#include "adc.h"
#include "gpio.h"
#include "sys.h"
#include "timer.h"
#include "uart.h"
#include "oled.h"

#define NUM_PINS 1

typedef enum
{
  HEARTBEAT_LED
} pin_t;

// configure GPIO pins
pin_config_t pins[NUM_PINS] = {

    // HEARTBEAT_LED
    {
        PB2,    // pin offset
        &PORTB, // port
        OUTPUT, // pin direction
        false,  // internal pullup
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

  OLED_init();

  ADC_calibrate_joystick();

  printf("\r\nSetup complete\r\n");

  while (1)
  {

    OLED_clear(); 

    OLED_write_char('B',0,0);
    OLED_write_char('u',1,1);
    OLED_write_char('s',2,2);

    OLED_refresh();

       // joystick_pos_t pos = ADC_get_joystick_pos();
    // printf("\033[2J\033[2;0H\r  ");
    // printf("dir: %d slider1: %d slider2: %d \r\n", ADC_calc_joystick_dir(pos),
    //        ADC_get_slider_pos(SLIDER_LEFT_INDEX),
    //        ADC_get_slider_pos(SLIDER_RIGHT_INDEX));

    // _delay_ms(20);
  }
}
