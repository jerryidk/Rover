#include "main.h"

volatile uint32_t tick = 0; // 1 tick = 100 ms
volatile int16_t dps = 0;
volatile int16_t orientation = 0; // degree 
volatile uint16_t front = 0xFFFF; // cm
volatile uint16_t left = 0xFFFF;
volatile uint16_t right = 0xFFFF;
volatile uint8_t pwm = 90;

void info_init(void);
void debug(void);
void delay(int t); 

int main(void)
{
  // Configure Systick to be 100ms, 10HZ
  SysTick_Config(800000);

#ifdef USART
  usart_init(9600);
  info_init();
#endif

#ifdef GYRO
  gyro_init();
  gyro_enable();
#endif

#ifdef HCSR
  hcsr_init();
#endif

#ifdef MOTOR
  motor_init();
#endif

#ifdef DEBUG
  debug();
#endif

  //Clear the screen
  usart_write_str("\033[2J");
  char c;

  // default motor actions
  bool execute_motor_instructions = false;
  Action_t action = GO_FORWARD;
  uint32_t duration = 0;
  uint8_t pwm_step = 10;

  while (1)
  {
      c = usart_read_byte(); 

      switch(c){
        
        case 'l':
          action = GO_LEFT;
          execute_motor_instructions = true;
          break;
        case 'r':
          action = GO_RIGHT;
          execute_motor_instructions = true;
          break;
        case 'u':
          action = GO_FORWARD;
          execute_motor_instructions = true;
          break;
        case 'd':
          action = GO_BACKWARD;
          execute_motor_instructions = true;
          break;

        case '1': // intentional fall through
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
        case '0':
          duration = (uint32_t)atoi(c); // atoi converts string to integer
          break;
        
        case '+':
          pwm = pwm + pwm_step;
          break;
        case '-':
          pwm = pwm - pwm_step;
          break;
        
        case '':
        default:
          break;
      }
      
      if (execute_motor_instructions == true)
      {
        motor_drive(pwm, duration, action);
        execute_motor_instructions = false;
      }

  }
}

/*
 * System tick happens every 100ms, it will update data from all sensors.
 *
 * Note: this interrupt should be atomic, where it shouldn't be interrupt away
 * by other interrupts.
 *
 * Processing this interrupt could take upto 15 - 20ms, which is < 100ms.
 * So it should never overlap itself.
 *
 */
void SysTick_Handler(void)
{
  __disable_irq();

  if (tick == 0xFFFFFFFF)
    tick = 0;
  else
    tick++;

#ifdef GYRO
  dps = gyro_z();
  orientation += dps / 10;
#endif

#ifdef HCSR
  front = hcsr_distance(0);
  left = hcsr_distance(1);
  right = hcsr_distance(2);
#endif
  
  __enable_irq();
}

/**
 * Initilize timer 2 to print out info.
*/
void info_init(void){

    RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;

    NVIC_SetPriority(TIM2_IRQn, 2);
    NVIC_EnableIRQ(TIM2_IRQn);
    
    TIM2->PSC = 40000-1;
    TIM2->ARR = 200;
    TIM2->DIER |= TIM_DIER_UIE;
    TIM2->CR1 = TIM_CR1_CEN;
}

void TIM2_IRQHandler() 
{
    __disable_irq();
    //clear 
    usart_write_str("\033[2J");
    //place cursor at (0,0)  
    usart_write_str("\033[0;0H");
    usart_printf("tick: %d\r\n"
                  "pwm: %d\r\n"
                  "dps: %d\r\n"
                  "o: %d\r\n"
                  "front: %d\r\n"
                  "left: %d\r\n"
                  "right: %d\r\n", 
                  tick, 
                  pwm,
                  dps, 
                  orientation, 
                  front, 
                  left, 
                  right);
    TIM2->SR &= ~TIM_SR_UIF;
    __enable_irq();
}

// Just a delay loop
void delay(int t)
{
  for (volatile int i = 0; i < t; i++)
    __NOP();
}

void debug()
{
  while(1);
}