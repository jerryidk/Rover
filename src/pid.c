#include "pid.h"

extern volatile uint32_t left_rev;
extern volatile uint32_t right_rev;

volatile uint32_t target_distance = 100; // cm
volatile bool PI_done = false;

const uint8_t Kp = 3;

void PI_init(void)
{
  PI_done = false;
  left_rev = 0;
  right_rev = 0;
  timer2_init();
  while (!PI_done)
    ;
}

int clamp(int v, int l, int h)
{
  if (v > h)
    return h;

  if (v < l)
    return l;

  return v;
}

/**
 * Initilize timer 2 to PI
 */
void timer2_init(void)
{

  RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;

  NVIC_SetPriority(TIM2_IRQn, 2);
  NVIC_EnableIRQ(TIM2_IRQn);
  TIM2->PSC = 4000 - 1;
  TIM2->ARR = 2000;
  TIM2->DIER |= TIM_DIER_UIE;
  TIM2->CR1 |= TIM_CR1_CEN;
}

/* 
* PI control
*/
void TIM2_IRQHandler(void)
{

  uint32_t rev = (left_rev + right_rev) / 2;
  uint32_t feedback_distance = (22 * rev - (rev / 100));

  int32_t error = (int32_t)(target_distance - feedback_distance);

  uint32_t output;
  if (error < 1)
  {
    PI_done = true;
    output = 0;
    TIM2->CR1 &= ~TIM_CR1_CEN;
  }
  else
  {
    output = (uint8_t)clamp(error * Kp, 90, 100);
  }

  usart_printf("error: %d\r\n", error);
  usart_printf("output: %d\r\n", output);

  motor_left_pwm(output, ROT_FORWARD);
  motor_right_pwm(output, ROT_FORWARD);

  TIM2->SR &= ~TIM_SR_UIF;
}