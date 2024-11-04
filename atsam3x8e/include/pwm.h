#ifndef __PWM__
#define __PWM__

#include <stdint.h>

void PWM_init(uint16_t period);
void PWM_stop();
void PWM_start();
void PWM_set_duty_cycle(uint16_t duty_cycle);

#endif // __PWM__
