#include "main.h"


void clear(char* buf, uint8_t size){

  for(int i=0; i<size; i++)
    buf[i] = (char) 0;
}
/**
 * Main program.
 * Default: 8MHz clock
 */
int main(void)
{ 
  // Enable system clock
  SysTick_Config(1000); // 1ms  
  RCC->AHBENR |= RCC_AHBENR_GPIOCEN;
  RCC->APB1ENR |= RCC_APB1ENR_USART3EN;

  // Usart pins 
  GPIO_PIN_T pin_usart_tx;
  pin_usart_tx.gpio = GPIOC;
  pin_usart_tx.pin = USART3_TX;
  pin_usart_tx.mode = MODE_AF;
  pin_usart_tx.af = 1;
  GPIO_init(&pin_usart_tx);

  GPIO_PIN_T pin_usart_rx;
  pin_usart_rx.gpio = GPIOC;
  pin_usart_rx.pin = USART3_RX;
  pin_usart_rx.mode = MODE_AF;
  pin_usart_rx.af = 1;
  GPIO_init(&pin_usart_rx);

  usart3_init(9600);
  char byte; 
  // Application
  while(1){
    
    byte = usart3_read_byte(); 
    usart3_write_byte(byte);
  } 
}
