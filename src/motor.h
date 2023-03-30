#ifndef MOTOR_H
#define MOTOR_H

#include <stdint.h>
#include "stm32f072xb.h"

/**
 *  TODO: Motor module
 * 
 *  
 *  Motor Pin: Black, Red, Input <- from HALL  
 *  
 *  PID system to output distance
 * 
 *  Error = CNT 
 *  
 *  Use a basic timer to configure a interrupt generated - dt
 *  Upon every timer interrupt:
 *      Get input count
 *  
 *  
 *  
*/

void motor_init();
void encoder_init();
void drive(int distance); 

#endif