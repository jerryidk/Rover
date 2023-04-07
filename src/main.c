#include "main.h"

volatile uint32_t tick = 0; // 1 tick = 100 ms
volatile int16_t dps = 0;
volatile int16_t orientation = 0; // degree
volatile uint16_t front = 0xFFFF; // cm
volatile uint16_t left = 0xFFFF;
volatile uint16_t right = 0xFFFF;
volatile uint16_t pwm = 30;

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

int main(void)
{
  // Configure Systick to be 100ms, 10HZ
  SysTick_Config(800000);

#ifdef USART
  usart_init(9600);
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
  while (1)
  {
      c = usart_read_byte(); 

      switch(c){

        case 'l':
          motor_left_pwm(pwm);
          break;
        case 'r':
          motor_right_pwm(pwm);
          break;
        case '+':
          pwm++;
          break;
        case '-':
          pwm--; 
          break;
        default:
          break;
      }

      usart_write_str("driving"); 
  }
}

/**
 * TODO: Write PI controller to run the rover in given distance
 *
 * target_distance.
 */
void PI_control()
{
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

#ifdef USART

    // Place cursor at (0,0)
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
#endif
  __enable_irq();
}
