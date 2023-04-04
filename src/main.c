#include "main.h"

uint32_t SystemClkFreq = 8000000; // in HZ.
uint8_t update_interval = 20;    // ms
volatile uint32_t tick = 0;       // 1 tick = 20 ms
volatile int16_t  dps = 0; 
volatile int16_t  orientation = 0; // degree
volatile uint16_t front = 0xFFFF;  // cm
volatile uint16_t left  = 0xFFFF;   
volatile uint16_t right = 0xFFFF;

void delay(){

  for(volatile int i=0; i<100000; i++) 
    __NOP(); 
}

int main(void)
{
  SysTick_Config(800000); 

  rcc_init(); 
  usart_init(9600);
  gyro_init();
  gyro_enable();
  hcsr_init();

  char c;
  //usart_write_str("\033[2J");
  
  usart_write_str("\033[2J");
  while (1)
  {

    delay();
    usart_write_str("\033[0;0H");
    usart_printf("tick: %d\r\ndps: %d\r\no:%d\r\nfront:%d\r\n", tick, dps, orientation, front);
  }
}

void SysTick_Handler(void)
{
    if(tick == 0xFFFFFFFF)
      tick = 0; 
    else
      tick++;

    dps = gyro_z(); 
    orientation += dps / 10;
    front = hcsr_distance(FRONT);
}
