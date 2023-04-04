/**
 *  
 *  Motor Module
 * 
 * PIN:
 * 
 * | motor1     | PA0  | OUT  | BLK     |
 * | motor1     | PA1  | OUT  | RED     | 
 * | motor1     | PB10 | AF2  | ENCODER |
 * | motor1     | PB1  | AF0  | PWM     |

 * | motor2     | PC7  | OUT  | BLK     |
 * | motor2     | PA3  | OUT  | RED     |
 * | motor2     | PC6  | AF0  | ENCODER |
 * | motor2     | PA2  | AF0  | PWM     |
 * 
 * TIMER: 
 *
 * | 2     | Encoder | TBD | PB10 | Motor1 encoder|
 * | 3     | Encoder | TBD | PC6  | Motor2 encoder|
 * | 14    | PMW     | TBD | PB1  | Motor pwm|
 * | 15    | PMW     | TBD | PA2  | Motor pwm|
 *  
 *  
*/

#ifndef MOTOR_H
#define MOTOR_H

#include <stdint.h>
#include "stm32f072xb.h"

void motor_init();
void motor1_init();
void motor2_init();
void encoder1_init();
void encoder2_init();

#endif
