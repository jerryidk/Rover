#ifndef PID_H
#define PID_H

#include "usart.h"
#include "motor.h"
#include <stdbool.h>
void timer2_init(void);
int clamp(int, int, int);
void PI_init(void);
void PI_finish();

#endif