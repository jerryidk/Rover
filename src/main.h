#ifndef MAIN_H
#define MAIN_H

/**
 *  Use Macro to turn on/off a certain module.
*/
#define USART
#define GYRO
#define HCSR
#define MOTOR
//#define DEBUG


#include <stdint.h>
#include <stdlib.h>
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

#endif
