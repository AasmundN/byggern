#include "joystick.h"
#include "adc.h"

#define NEUTRAL_RADIUS 50 // circle around joystick position (0,0)
#define SAMPLE_BATCH_SIZE 10

uint8_t joystick_x_pos_neutral;
uint8_t joystick_y_pos_neutral;

uint8_t adc_buffer[NUM_ADC_CHANNELS];

void JOYSTICK_calibrate()
{
  ADC_read(adc_buffer);

  joystick_x_pos_neutral = adc_buffer[JOYSTICK_X_INDEX];
  joystick_y_pos_neutral = adc_buffer[JOYSTICK_Y_INDEX];
}

/**
 * Read the ADC a few times and calculate the average. Map averages to -100
 * to 100 while accounting for stick drift. I.E. the ADC reading corresponding
 * with 0 is saved in joystick_x_pos_neutral.
 */
joystick_pos_t JOYSTICK_get_pos()
{

  uint16_t x_sum = 0, y_sum = 0;
  uint8_t x_average = 0, y_average = 0;

  for (int i = 0; i < SAMPLE_BATCH_SIZE; i++)
  {
    ADC_read(adc_buffer);
    x_sum += adc_buffer[JOYSTICK_X_INDEX];
    y_sum += adc_buffer[JOYSTICK_Y_INDEX];
  }

  x_average = x_sum / SAMPLE_BATCH_SIZE;
  y_average = y_sum / SAMPLE_BATCH_SIZE;

  int x =
      x_average <= joystick_x_pos_neutral
          ? (x_average - joystick_x_pos_neutral) * 100 / joystick_x_pos_neutral
          : (x_average - joystick_x_pos_neutral) * 100
                / (255 - joystick_x_pos_neutral);

  int y =
      y_average <= joystick_y_pos_neutral
          ? (y_average - joystick_y_pos_neutral) * 100 / joystick_y_pos_neutral
          : (y_average - joystick_y_pos_neutral) * 100
                / (255 - joystick_y_pos_neutral);

  joystick_pos_t result = {x, y};

  return result;
}

joystick_dir_t JOYSTICK_calc_dir(joystick_pos_t pos)
{
  // check neutral position circle
  if ((pos.x * pos.x + pos.y * pos.y) < NEUTRAL_RADIUS * NEUTRAL_RADIUS)
    return NEUTRAL;

  if (pos.x > pos.y)
    return -pos.x < pos.y ? RIGHT : DOWN;

  return -pos.x > pos.y ? LEFT : UP;
}
