#include "main.h"


/**
 * Main program.
 * Default: 8MHz clock
 */
int main(void)
{
  // Enable system clock
  SysTick_Config(1000); 
  // Usart module
  usart_module_init(); 
  Gyro_init();
  Gyro_enable();
  int16_t dps;
  int16_t o = 0;

  // configure a timer interrupt to update orientation. 
  while (1)
  {
    delay(100);
    dps = Gyro_z() * 1000 / 100000;
    o += dps / 10; 
    usart3_write_num(o);
    usart3_write_byte('\n');
  }
}

void usart_module_init()
{
  RCC->AHBENR |= RCC_AHBENR_GPIOCEN;
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
}
