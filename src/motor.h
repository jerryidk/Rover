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

/*
| motor1     | PC2  | OUT  | RED     |
| motor1     | PC3  | OUT  | BLK     |
| motor1     | PC6  | AF0  | ENCODER | TIM3_CH1  |
| motor1     | PB8  | AF2  | PWM     | TIM16_CH1  |

| motor2     | PC4  | OUT  | RED     |
| motor2     | PC5  | OUT  | BLK     |
| motor2     | PB9  | AF2  | PWM | TIM17_CH1  |
*/

void motor_init();
void motor_left_pwm(uint8_t pwm);
void motor_right_pwm(uint8_t pwm);
void motor_drive(uint8_t speed, uint32_t time, Action_t action); // time in ms, speed in duty cycle

#endif
