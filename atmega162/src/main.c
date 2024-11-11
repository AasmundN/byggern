#define F_CPU 4900000UL

#include <avr/interrupt.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <util/delay.h>
#include <string.h>

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

  can_msg_t received_can_msg;

  joystick_dir_t dir_prev = NEUTRAL;
  time_t currentTime;
  char timeStr[10];
  uint8_t nameIndex = 0;
  char name[4] = "AAA";  

  while (1)
  {
    joystick_pos_t pos = ADC_get_joystick_pos();
    joystick_dir_t dir = ADC_calc_joystick_dir(pos);
    int joystick_btn = GPIO_read_pin(&pins[JOYSTICK_BTN]);
    int slider_btn = GPIO_read_pin(&pins[TOUCH_BTN_LEFT]);

    input_data.joystick_dir = dir;
    input_data.joystick_pos = pos;
    input_data.joystick_btn_state = joystick_btn;

    input_data.slider_pos = ADC_get_slider_pos(SLIDER_LEFT_INDEX);
    input_data.slider_btn_state = slider_btn;


    switch (game_menu.state) { 
      case GAME:
        CAN_transmit(&input_data_can_msg);

        OLED_clear_screen();
        OLED_print("Game running", 0,0);

        time_t currentTimeLocal = TIMER_get_game_time();
        memcpy(&currentTime, &currentTimeLocal, sizeof(time_t));

        sprintf(timeStr, "%2u:%2u", currentTime.minutes,currentTime.seconds);
        OLED_print("Time: ", 3, 0);
        OLED_print(timeStr,3,6);

        OLED_refresh();
        break;

      case MENU:
        if(dir!=dir_prev)
          MENU_update_state(&game_menu, dir); 

        MENU_draw_page(game_menu.current);
        dir_prev = dir;

        break;

      case GAME_OVER:
        OLED_clear_screen();

        OLED_print("Game over",0,0);
        OLED_print("Time: ", 3, 0);
        OLED_print(timeStr,3,6);

        OLED_print("Enter name:", 5,0);
        OLED_print(name, 6,0);
        OLED_write_char(MOGUS, 7, nameIndex);

        if(!joystick_btn){
          game_menu.state = MENU;
          int leaderboardIndex = 4;

          for(int i=0;i<4; i++){
            if(leaderboard[i].score.minutes < currentTime.minutes)
            {
              leaderboardIndex = i;
              break;
            }

            if(leaderboard[i].score.minutes==currentTime.minutes && leaderboard[i].score.seconds<currentTime.seconds)
            {
              leaderboardIndex = i;
              break;
            }
          }

          if(leaderboardIndex<4){
            for(int i=3; i>leaderboardIndex; i--){
              memcpy(&leaderboard[i], &leaderboard[i-1], sizeof(score_t));
            }

            memcpy(&leaderboard[leaderboardIndex].name, &name, sizeof(char)*4);
            memcpy(&leaderboard[leaderboardIndex].score, &currentTime, sizeof(time_t));
          }

          game_menu.state=MENU;
          for(int i=0; i<4; i++){
            printf("#%u %s %2u:%2u \r\n",(i+1), leaderboard[i].name, leaderboard[i].score.minutes, leaderboard[i].score.seconds);
          }
        }

        if(dir!=dir_prev)
          switch (dir) {
            case LEFT:
              nameIndex = (nameIndex-1)%3;
              break;

            case RIGHT:
              nameIndex = (nameIndex+1)%3;
              break;
              
            case UP:
              name[nameIndex]--;
              break;  

            case DOWN:
              name[nameIndex]++;
              break;
          }

        dir_prev=dir;
        OLED_refresh();

        break;

      default:
        break;
    }
    if(slider_btn)
      game_menu.state = MENU;

    if (!CAN_receive(&received_can_msg))
    {
      printf("ID: %d\r\n", received_can_msg.id);
      if(received_can_msg.id==END_GAME_ID && game_menu.state==GAME)
        game_menu.state = GAME_OVER;
    }

    _delay_ms(10);
  }
}
