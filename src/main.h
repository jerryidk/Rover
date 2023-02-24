#ifndef MAIN_H
#define MAIN_H

#include <stdint.h>
#include "stm32f072xb.h"
#include "timer.h"
#include "usart.h"
#include "gpio.h"
#include "hcsr.h"

#define REDLED (6)
#define BLUELED (7)
#define ORANGELED (8)
#define GREENLED (9)
#define USART3_TX (10) 
#define USART3_RX (11)


uint32_t SystemClkFreq = 8000000; // in HZ.

/**
 * Basic support. 
 * Be careful with perepherial, 
 * this only set system clock to HSI48, 
 * doesn't config the RCC bus at all. 
*/
void HSI48_EN(void)
{
  // Flash configuration
  FLASH->ACR |= (FLASH_ACR_LATENCY | FLASH_ACR_PRFTBE);	
  RCC->CR2 |= RCC_CR2_HSI48ON; // hsi48 on
  // See if HSI is ready to be selected 
  while((RCC->CR2 & RCC_CR2_HSI48RDY) != RCC_CR2_HSI48RDY);
  /* Select HSI48 as system clock source */
  RCC->CFGR |= RCC_CFGR_SW_HSI48;
  /* Wait for HSI48 clock to be selected as system clock */
  while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_HSI48);  

  SystemClkFreq = 48000000; 
}

extern volatile uint32_t tick;

uint8_t delay(uint32_t t)
{ 
  NVIC_DisableIRQ(SysTick_IRQn); 
  uint32_t start = tick;
  if (start > (UINT32_MAX - t)) {
    // overflow 
    return 0;
  }
  uint32_t end = start + t;
  NVIC_EnableIRQ(SysTick_IRQn); 
  while (tick<end);
  return 1;
}

#endif
