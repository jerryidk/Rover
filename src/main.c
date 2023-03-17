#include "main.h"


/**
 * TODO
 *  
 * 
*/

/**
 * Main program.
 * Default: 8MHz clock
 * 
 *          call HSI48_EN if in needs of 48MHz clock (for lab purpose, you don't need it! ).
 *          consult rcc perepheral for more info.
 */
int main(void)
{ 
  // Enable system clock
  SysTick_Config(1000); // 1ms  
  RCC->AHBENR |= RCC_AHBENR_GPIOCEN;

  /*
    Initialize all perepheral
     1. GPIO   
     2. Usart
     3. Timers
     4. SPI
     5. I2C

  *  PIN descriptions:
  *  
  *      Name               PIN                 
  *       
  *      USART_TX                             
  *      USART_RX 
  * 
  *      HCSR1_IN
  *      HCSR1_OUT
  * 
  *      HCSR2_IN
  *      HCSR2_OUT
  * 
  *      HCSR3_IN
  *      HCSR3_OUT
  *   
  *      MOTOR1_RED
  *      MOTOR1_BLK
  *      MOTOR1_EN
  *      HALL_ENC_IN
  *      SCL
  *      SDA
  *      OS
  *      
  *     
  *            
  */

  GPIO_PIN_T pin_usart_tx;
  pin_usart_tx.gpio = GPIOC;
  pin_usart_tx.pin = USART3_TX;
  pin_usart_tx.mode = MODE_AF;
  pin_usart_tx.af = 1;
  GPIO_init(&pin_usart_tx);
  usart3_init(9600);
  usart3_write("usart init... \r\n");

  // PIN 0 Input Echo
  GPIO_PIN_T pin_echo;
  pin_echo.gpio = GPIOC;
  pin_echo.pin = 0;
  pin_echo.mode = MODE_IN;
  pin_echo.pupd = PULL_DOWN;
  GPIO_init(&pin_echo);

  // PIN 1 Output Trigger
  GPIO_PIN_T pin_trigger;
  pin_trigger.gpio = GPIOC;
  pin_trigger.pin = 6;
  pin_trigger.mode = MODE_OUT;
  pin_trigger.ospeed = 1;
  pin_trigger.pupd = PULL_DOWN; 
  GPIO_init(&pin_trigger); 

  BASIC_TIMER_T timer6;
  timer6.tim = TIM6;
  timer6.psc = 8 - 1;
  timer6.arr = UINT16_MAX;
  Timer_init(&timer6);

  HCSR_T hscr;
  hscr.echo = &pin_echo;
  hscr.trigger = &pin_trigger;
  hscr.timer = &timer6;
  if(!HCSR_check(&hscr))
    usart3_write("!! ERROR: init hcsr\n"); 

   // Never returns 
  while(1){
    usart3_write_num(HCSR_distance(&hscr));
    delay(1000);
  } 
}
