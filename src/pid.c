#include "pid.h"

extern volatile uint32_t left_cnt;
extern volatile uint32_t right_cnt;
extern volatile uint32_t orientation;

volatile uint32_t target_distance = 100; // cm
volatile bool PI_done = false;

const uint8_t Kp = 3;

void PI_init(void)
{
  PI_done = false;
  left_cnt = 0;
  right_cnt = 0;
  timer2_init();
  char c; 
  while (!PI_done)
  {
    if(data_ready()>0 && (c = read_byte()) == '/'){
      PI_finish();
      break;
    }
  }
}

int clamp(int v, int l, int h)
{
  if (v > h)
    return h;

  if (v < l)
    return l;

  return v;
}


void PI_finish() 
{
      PI_done = true;
      motor_left_pwm(0, ROT_FORWARD);
      motor_right_pwm(0, ROT_FORWARD);
      TIM2->DIER &= ~TIM_DIER_UIE;
      TIM2->CR1 &= ~TIM_CR1_CEN;
      TIM2->SR &= ~TIM_SR_UIF;
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
  TIM2->ARR = 200;
  TIM2->DIER |= TIM_DIER_UIE;
  TIM2->CR1 |= TIM_CR1_CEN;
}

/* 
* PI control
*/
void TIM2_IRQHandler(void)
{

  uint32_t cnt = (left_cnt + right_cnt) / 2;
  // 5.3 cm per cnt
  uint32_t feedback_distance = 5 * cnt + 4 * (cnt / 10);
  int32_t error = (int32_t)(target_distance - feedback_distance);

  uint32_t output;
  if (error < 1)
  {
    PI_finish();
    return;
  }
  
  output = (uint8_t)clamp(error * Kp, 90, 100);
  uint32_t l_output = output;
  uint32_t r_output = output;
  // if(left_cnt > right_cnt)
  //     l_output--;
  // if(left_cnt < right_cnt)
  //     r_output--;
  
  usart_printf("error: %d\r\n", error);
  usart_printf("l output: %d\r\n", l_output);
  usart_printf("r output: %d\r\n", r_output);
  usart_printf("left cnt: %d\r\n", left_cnt);
  usart_printf("right cnt: %d\r\n", right_cnt);
  
  motor_left_pwm(l_output, ROT_FORWARD);
  motor_right_pwm(r_output, ROT_FORWARD);
  TIM2->SR &= ~TIM_SR_UIF;
}