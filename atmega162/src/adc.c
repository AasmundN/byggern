#define ADC_START_ADDR 0x1400
#define T_CONV 32        // microseconds
#define NEUTRAL_RADIUS 2 // circle around joystick position (0,0)

#include <avr/io.h>
#include <util/delay.h>

#include "adc.h"

uint8_t joystick_x_pos_neutral;
uint8_t joystick_y_pos_neutral;

uint8_t adc_buffer[NUM_ADC_CHANNELS];

/**
 * Read all ADC channels
 * @param result read result is placed here
 */
void ADC_read(uint8_t *result)
{
  volatile uint8_t *adc = (uint8_t *)ADC_START_ADDR;

  // Initiate ADC conversion
  *adc = '\0';

  // Wait for conversion to complete
  _delay_us(T_CONV);

  // Read all ADC channels
  for (uint8_t i = 0; i < NUM_ADC_CHANNELS; i++)
    result[i] = *adc;
}

void ADC_calibrate_joystick()
{
  ADC_read(adc_buffer);

  joystick_x_pos_neutral = adc_buffer[JOYSTICK_X_INDEX];
  joystick_y_pos_neutral = adc_buffer[JOYSTICK_Y_INDEX];
}

joystick_pos_t ADC_get_joystick_pos()
{
  ADC_read(adc_buffer);

  /**
   * Map joystick ADC readings to -100 to 100 while
   * accounting for stick drift. I.E. the ADC reading corresponding
   * with 0 is saved in joystick_x_pos_neutral.
   */

  int x = adc_buffer[JOYSTICK_X_INDEX] <= joystick_x_pos_neutral
              ? (adc_buffer[JOYSTICK_X_INDEX] - joystick_x_pos_neutral) * 100 /
                    joystick_x_pos_neutral
              : (adc_buffer[JOYSTICK_X_INDEX] - joystick_x_pos_neutral) * 100 /
                    (255 - joystick_x_pos_neutral);

  int y = adc_buffer[JOYSTICK_Y_INDEX] <= joystick_y_pos_neutral
              ? (adc_buffer[JOYSTICK_Y_INDEX] - joystick_y_pos_neutral) * 100 /
                    joystick_y_pos_neutral
              : (adc_buffer[JOYSTICK_Y_INDEX] - joystick_y_pos_neutral) * 100 /
                    (255 - joystick_y_pos_neutral);

  joystick_pos_t result = {x, y};

  return result;
}

int ADC_get_slider_pos(int slider_index)
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
