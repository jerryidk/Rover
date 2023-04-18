#include "hall.h"

volatile uint32_t left_rev = 0;
volatile uint32_t right_rev = 0;

void hall_init()
{
  RCC->APB2ENR|= RCC_APB2ENR_SYSCFGCOMPEN;
  SYSCFG->EXTICR[3] |= SYSCFG_EXTICR4_EXTI14_PC | SYSCFG_EXTICR4_EXTI15_PC;
  
  // Enable EXTI line 0  
  EXTI->IMR  |= (1 << 14) | (1 << 15); 
  EXTI->FTSR |= (1 << 14) | (1 << 15); 


  NVIC_EnableIRQ(EXTI4_15_IRQn);
  NVIC_SetPriority(EXTI4_15_IRQn, 1);
}

void EXTI4_15_IRQHandler(void)
{
    if (EXTI->PR & (1 << 14)) {
       right_rev++;
       // clear pending by writing 1 to locaiton.
       EXTI->PR |= (1 << 14); 
    }

    if (EXTI->PR & (1 << 15)) {
       left_rev++;
       EXTI->PR |= (1 << 15); 
    }
}
 