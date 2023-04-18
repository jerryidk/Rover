#ifndef MAIN_H
#define MAIN_H

/**
 *  Use Macro to turn on/off a certain module.
*/
#define USART
#define GYRO
#define HCSR
#define MOTOR
#define HALL
//#define DEBUG


#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "stm32f072xb.h"


#ifdef USART
#include "usart.h"
#endif

#ifdef HCSR
#include "hcsr.h"
#endif

#ifdef GYRO
#include "gyro.h"
#endif

#ifdef MOTOR
#include "motor.h"
#endif

#ifdef HALL
#include "hall.h"
#endif

#endif
