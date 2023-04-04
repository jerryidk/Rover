#include "rcc.h"

void rcc_init()
{
    RCC->AHBENR |= RCC_AHBENR_GPIOCEN |
                   RCC_AHBENR_GPIOBEN |
                   RCC_AHBENR_GPIOAEN;

    RCC->APB1ENR |= RCC_APB1ENR_SPI2EN |
                    RCC_APB1ENR_USART3EN |
                    RCC_APB1ENR_TIM14EN | 
                    RCC_APB1ENR_TIM7EN |
                    RCC_APB1ENR_TIM6EN |
                    RCC_APB1ENR_TIM3EN |
                    RCC_APB1ENR_TIM2EN;
}