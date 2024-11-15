#ifndef __JOYSTICK__
#define __JOYSTICK__

#include <stdint.h>

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
void JOYSTICK_calibrate();

/**
 * Get position of joystick
 * @return position struct; x/y are mapped -100 to 100
 */
joystick_pos_t JOYSTICK_get_pos();

/**
 * Calculate direction of joystick
 * @param jostick pos
 * @return joystick direction
 */
joystick_dir_t JOYSTICK_calc_dir(joystick_pos_t pos);

#endif // !__JOYSTICK__
