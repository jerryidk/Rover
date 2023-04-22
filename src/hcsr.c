#include "hcsr.h"
uint16_t tmax = 7000; // max time processor should wait on hcsr.

void hcsr_init()
{

    RCC->APB1ENR |= RCC_APB1ENR_TIM7EN;
    RCC->AHBENR |= RCC_AHBENR_GPIOAEN | RCC_AHBENR_GPIOBEN;

    // Initilize timer 7, freq = 1MHZ
    TIM7->PSC = 7;

    // Arr should be t_max.
    TIM7->ARR = (uint16_t)tmax;
    TIM7->CNT = 0;
    
    GPIOA->MODER |= GPIO_MODER_MODER8_0 |
                    GPIO_MODER_MODER9_0 |
                    GPIO_MODER_MODER10_0;

    GPIOA->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR8_0 | 
                      GPIO_OSPEEDER_OSPEEDR8_1 |
                      GPIO_OSPEEDER_OSPEEDR9_0 | 
                      GPIO_OSPEEDER_OSPEEDR9_1 |
                      GPIO_OSPEEDER_OSPEEDR10_0 | 
                      GPIO_OSPEEDER_OSPEEDR10_1 ;

    GPIOB->PUPDR |= GPIO_PUPDR_PUPDR3_1 | 
                    GPIO_PUPDR_PUPDR4_1 | 
                    GPIO_PUPDR_PUPDR5_1; 
}

// Note, this function is time senstive, we need to disable interrupts
uint16_t hcsr_distance(uint8_t offset)
{
    // PIN number
    uint8_t trigger_pin= 8+offset;
    uint8_t echo_pin = 3+offset;
    
    uint16_t t = 0;

    uint32_t echo_msk = (1 << echo_pin);
    uint32_t trigger_set_msk = (uint32_t) (1 << (trigger_pin));  
    uint32_t trigger_reset_msk = (uint32_t) (1 << (trigger_pin+16));  

    // Turn trigger line high
    GPIOA->BSRR = trigger_set_msk;
    TIM7->CNT = 0;
    TIM7->CR1 |= TIM_CR1_CEN;
    while (TIM7->CNT < 50)
        ;
    // Turn trigger line low
    GPIOA->BSRR = trigger_reset_msk;
    TIM7->CR1 &= ~TIM_CR1_CEN;

    // Wait for echo to be high
    TIM7->CNT = 0;
    TIM7->SR &= ~TIM_SR_UIF;
    TIM7->CR1 |= TIM_CR1_CEN;
    while (1)
    {
        if ((GPIOB->IDR & echo_msk))
            break;
        // wait up to 5ms.
        if (TIM7->SR & TIM_SR_UIF)
        {
            // probably nothing
            TIM7->CR1 &= ~TIM_CR1_CEN;
            return 0;
        }
    }
    
    // Start timing
    TIM7->CNT = 0;
    TIM7->SR &= ~TIM_SR_UIF;
    TIM7->CR1 |= TIM_CR1_CEN;
    // Wait for echo to be low or timer overflow
    while (1)
    {
        if ((GPIOB->IDR & echo_msk) == 0)
        {
            t = TIM7->CNT;
            break;
        }

        if (TIM7->SR & TIM_SR_UIF)
        {
            t = tmax;
            break;
        }
    };

    TIM7->CR1 &= ~TIM_CR1_CEN;

    return t / 59;
}
