
#include "timer.h"

void Timer_init(BASIC_TIMER_T *timer)
{
    timer->tim->PSC = timer->psc;
    timer->tim->ARR = timer->arr;
}

void Timer_start(BASIC_TIMER_T *timer)
{
    timer->tim->CNT = 0;
    timer->tim->CR1 |= TIM_CR1_CEN;
}

uint16_t Timer_getCnt(BASIC_TIMER_T *timer)
{
    // if timer overflow, return 0;
    return (uint16_t)timer->tim->CNT;
}

void Timer_end(BASIC_TIMER_T *timer)
{
    timer->tim->CR1 &= ~(TIM_CR1_CEN);
}

uint8_t Timer_delay(BASIC_TIMER_T *timer, uint16_t ticks)
{
    if( ticks > UINT16_MAX )
        return 0;
    if (ticks > (uint16_t)timer->arr)
        return 0;

    TIM_TypeDef * TIMER = timer->tim; 
    TIMER->CNT = 0;
    TIMER->CR1 |= TIM_CR1_CEN;
    while (TIMER->CNT < ticks)
        ;
    TIMER->CR1 &= ~(TIM_CR1_CEN);
    return 1;
}