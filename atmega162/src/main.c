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

#define NUM_PINS 1

typedef enum
{
  HEARTBEAT_LED
} pin_t;

// configure GPIO pins
pin_config_t pins[NUM_PINS] = {

    // HEARTBEAT_LED
    {
        PB5,    // pin offset
        &PORTB, // port
        OUTPUT, // pin direction
        false,  // internal pullup
    } // end HEARTBEAT_LED
};

volatile bool should_transmit = false;
volatile unsigned char received_ch;
uint8_t adc_result[NUM_ADC_CHANNELS];

void uart_receive_cb()
{
  received_ch = UART_receive();
  should_transmit = true;
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

  printf("\r\nSetup complete\r\n");

  while (1)
  {
    if (!should_transmit)
      continue;

    printf("\r\nReading ADC:\r\n");

    ADC_read(adc_result);

    for (uint8_t i = 0; i < NUM_ADC_CHANNELS; i++)
      printf("%hhu\r\n", adc_result[i]);

    should_transmit = false;
  }
}
