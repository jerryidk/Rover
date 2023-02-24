#ifndef TIMER_H
#define TIMER_H

#include <stdint.h>
#include "stm32f072xb.h"


typedef struct basic_timer{
    TIM_TypeDef*  tim;
    uint16_t psc; 
    uint16_t arr;
} BASIC_TIMER_T;

void Timer_init(BASIC_TIMER_T* timer);
void Timer_start(BASIC_TIMER_T* timer);
uint16_t Timer_getCnt(BASIC_TIMER_T* timer);
void Timer_end(BASIC_TIMER_T* timer);
uint8_t Timer_delay(BASIC_TIMER_T* timer, uint16_t ticks);
#endif