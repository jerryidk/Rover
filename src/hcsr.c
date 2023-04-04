#include "hcsr.h"

uint16_t tmax = 5000; // max time processor should wait. 

void hcsr_init()
{
    // Initilize timer 7, freq = 1MHZ
    TIM7->PSC = (SystemClkFreq / 1000000) - 1;

    // Arr should be t_max. 
    TIM7->ARR = (uint16_t)tmax;
    TIM7->CNT = 0;

    // PA 5 - 7 to be OUT
    GPIOA->MODER |= GPIO_MODER_MODER5_0 |
                    GPIO_MODER_MODER6_0 |
                    GPIO_MODER_MODER7_0;
}

// Note, this function is time senstive, we need to disable interrupts
uint16_t hcsr_distance(Direction_t dir)
{
    
    // PIN number
    uint8_t trigger = 5 + dir; 
    uint8_t echo = 3 + dir; 

    uint16_t echo_msk = (1 << echo);
    __disable_irq();
    uint16_t t = 0;

    // Trigger pin stay high for 15 us

    // Turn trigger line high
    GPIOA->BSRR = 0x1 << trigger;
    
    TIM7->CNT = 0;
    TIM7->CR1 |= TIM_CR1_CEN;
    while (TIM7->CNT < 15)
        ;
    TIM7->CR1 &= ~TIM_CR1_CEN;

    // Turn trigger line low 
    GPIOA->BSRR = 0x1 << (trigger + 16);

    // Wait for echo to be high
    while ((GPIOB->IDR & echo_msk ) == 0)
        ;
    
    TIM7->CNT = 0;
    TIM7->CR1 |= TIM_CR1_CEN;
    TIM7->SR  &= ~TIM_SR_UIF; 
    // Wait for echo to be low or timer overflow
    // 
    while ((TIM7->SR & TIM_SR_UIF) != 1 &&
           (GPIOB->IDR & echo_msk) != 0)
        ;
    
    if (TIM7->SR & TIM_SR_UIF )
        t = tmax;
    else
        t = TIM7->CNT;
        

    TIM7->CR1 &= ~TIM_CR1_CEN;
    __enable_irq();

    return t / 59;
}
