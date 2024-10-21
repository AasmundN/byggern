#ifndef __ADC__
#define __ADC__

#include <stdint.h>

#define ADC_REG (*(uint8_t *)0x1400)

#define NUM_ADC_CHANNELS 4

#define JOYSTICK_X_INDEX 0
#define JOYSTICK_Y_INDEX 1
#define SLIDER_RIGHT_INDEX 2
#define SLIDER_LEFT_INDEX 3

typedef struct
{
  int8_t x;
  int8_t y;
} joystick_pos_t;

typedef enum
{
  LEFT,
  RIGHT,
  UP,
  DOWN,
  NEUTRAL,
} joystick_dir_t;

/**
 * Read and save neutral position of joystick
 */
void ADC_calibrate_joystick();

/**
 * Get position of joystick
 * @return position struct; x/y are mapped -100 to 100
 */
joystick_pos_t ADC_get_joystick_pos();

/**
 * Get direction of joystick
 * @return joystick direction
 */
joystick_dir_t ADC_calc_joystick_dir(joystick_pos_t pos);

/**
 * Get slider postition
 * @param index of slider
 * @return slider position mapped to 0 to 100
 */
int8_t ADC_get_slider_pos(uint8_t slider_index);

#endif // __ADC__
