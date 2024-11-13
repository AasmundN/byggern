#include "motor.h"
#include "compiler.h"
#include "encoder.h"
#include "gpio.h"
#include "pwm.h"
#include "sam.h"
#include "tc.h"

#include <stdint.h>
#include <stdlib.h>

#define SAMPLING_PERIOD 262500 // 1ms

#define Kp 0.1
#define T 0.1 // 1ms in s
#define Ki 0.1

#define ENCODER_MAX 5000
#define ENCODER_MIN 0

#define MOTOR_PWM_PERIOD 210 // 50kHz

#define MOTOR_DIR_PIN PIOC, PIO_PC23

pin_config_t motor_pin = {
    MOTOR_DIR_PIN,
    .direction = OUTPUT,
    .pullup = false,
};

int8_t motor_pos = 0;

void MOTOR_controller_tick()
{
  static float error_sum = 0;

  int16_t encoder_value = ENCODER_read();

  float encoder_target = motor_pos * (ENCODER_MAX - ENCODER_MIN) / 100.0;
  float error = encoder_target - encoder_value;

  error_sum += error;

  float u = Kp * error + T * Ki * error_sum;

  if (u >= 0)
    GPIO_write(MOTOR_DIR_PIN, 0);
  else
    GPIO_write(MOTOR_DIR_PIN, 1);

  PWM_set_duty_cycle(MOTOR_PWM, min(abs((int16_t)u), MOTOR_PWM_PERIOD));
}

void MOTOR_init()
{
  PWM_init(MOTOR_PWM, MOTOR_PWM_PERIOD);
  PWM_set_duty_cycle(MOTOR_PWM, 0);
  PWM_start(MOTOR_PWM);

  GPIO_init(&motor_pin, 1);
  GPIO_write(MOTOR_DIR_PIN, 1);

  TC_init(SAMPLING_PERIOD); // controller sampling period
  TC_set_cb(MOTOR_controller_tick);

  ENCODER_init();
}

void MOTOR_start() { TC_enable(); }

void MOTOR_stop()
{
  TC_disable();
  PWM_set_duty_cycle(MOTOR_PWM, 0);
}

void MOTOR_set_pos(int8_t pos) { motor_pos = pos; }
