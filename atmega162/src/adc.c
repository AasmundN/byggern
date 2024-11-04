#include <avr/io.h>
#include <util/delay.h>

#include "adc.h"

#define T_CONV 32         // microseconds
#define NEUTRAL_RADIUS 50 // circle around joystick position (0,0)
#define SAMPLE_BATCH_SIZE 5

uint8_t joystick_x_pos_neutral;
uint8_t joystick_y_pos_neutral;

uint8_t adc_buffer[NUM_ADC_CHANNELS];

/**
 * Read all ADC channels
 * @param result read result is placed here
 */
void ADC_read(uint8_t *result)
{
  // Initiate ADC conversion
  ADC_REG = '\0';

  // Wait for conversion to complete
  _delay_us(T_CONV);

  // Read all ADC channels
  for (uint8_t i = 0; i < NUM_ADC_CHANNELS; i++)
    result[i] = ADC_REG;
}

void ADC_calibrate_joystick()
{
  ADC_read(adc_buffer);

  joystick_x_pos_neutral = adc_buffer[JOYSTICK_X_INDEX];
  joystick_y_pos_neutral = adc_buffer[JOYSTICK_Y_INDEX];
}

joystick_pos_t ADC_get_joystick_pos()
{
  /**
   * Averages joystick ADC readings every third iteration. Map averages to -100 to 100 while
   * accounting for stick drift. I.E. the ADC reading corresponding
   * with 0 is saved in joystick_x_pos_neutral.
   */
  
  uint16_t x_sum = 0, y_sum = 0;
  uint8_t x_average = 0, y_average = 0;

  for(int i=0;i<SAMPLE_BATCH_SIZE;i++) 
  {
    ADC_read(adc_buffer);
    x_sum += adc_buffer[JOYSTICK_X_INDEX];
    y_sum += adc_buffer[JOYSTICK_Y_INDEX];
  }

    x_average = x_sum/SAMPLE_BATCH_SIZE;
    y_average = y_sum/SAMPLE_BATCH_SIZE;

  int x = x_average <= joystick_x_pos_neutral
              ? (x_average - joystick_x_pos_neutral) * 100 /
                    joystick_x_pos_neutral
              : (x_average - joystick_x_pos_neutral) * 100 /
                    (255 - joystick_x_pos_neutral);

  int y = y_average <= joystick_y_pos_neutral
              ? (y_average - joystick_y_pos_neutral) * 100 /
                    joystick_y_pos_neutral
              : (y_average - joystick_y_pos_neutral) * 100 /
                    (255 - joystick_y_pos_neutral);

        
  joystick_pos_t result = {x, y};

  return result;
}

int8_t ADC_get_slider_pos(uint8_t slider_index)
{
  ADC_read(adc_buffer);

  return adc_buffer[slider_index] * 100 / 255;
}

joystick_dir_t ADC_calc_joystick_dir(joystick_pos_t pos)
{
  // check neutral position circle
  if ((pos.x * pos.x + pos.y * pos.y) < NEUTRAL_RADIUS * NEUTRAL_RADIUS)
    return NEUTRAL;

  if (pos.x > pos.y)
    return -pos.x < pos.y ? RIGHT : DOWN;

  return -pos.x > pos.y ? LEFT : UP;
}


