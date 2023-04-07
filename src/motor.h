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

void motor_init();
void motor_left_init();
void motor_right_init();
void motor_left_pwm(uint8_t pwm);
void motor_right_pwm(uint8_t pwm);
void motor_drive(uint8_t speed, uint32_t time, Action_t action); // time in ms, speed in duty cycle

#endif
