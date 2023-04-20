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
void motor_left_pwm(uint8_t, MOTOR_ROT_DIRECTION_t);
void motor_right_pwm(uint8_t, MOTOR_ROT_DIRECTION_t);
void motor_drive(uint8_t, uint32_t, Action_t);


#endif
