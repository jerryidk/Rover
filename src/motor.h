#ifndef MOTOR_H
#define MOTOR_H

#include <stdint.h>
#include "stm32f072xb.h"

typedef enum
{
    GO_LEFT = 0,
    GO_RIGHT = 1,
    GO_FORWARD = 2,
    GO_BACKWARD = 3,
} Action_t;

typedef enum
{
    ROT_FORWARD = 0,
    ROT_BACKWARD = 1,
} MOTOR_ROT_DIRECTION_t;

void motor_init();
void motor_left_init();
void motor_right_init();
void motor_left_pwm(uint8_t pwm);
void motor_right_pwm(uint8_t pwm);
void motor_drive(uint8_t speed, uint32_t duration, Action_t action);
void PI_control(void);



// Just a delay loop
extern volatile uint32_t tick;
void delay_ms(int t);
{
  int start = tick;
  while (tick - start < t)
  {
    if (tick > start && (tick - start >= t))
      break;
    if (tick < start && (start - tick >= t))
      break;
  }
}

#endif
