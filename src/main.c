#include "main.h"

volatile uint32_t tick = 0; // 1 tick = 100 ms
volatile int16_t dps = 0;
volatile int16_t orientation = 0; // degree
volatile uint16_t front = 0xFFFF; // cm
volatile uint16_t left = 0xFFFF;
volatile uint16_t right = 0xFFFF; 
volatile uint8_t pwm = 95;
volatile uint32_t duration = 200; // ms
volatile uint8_t p_step = 1;
volatile uint8_t d_step = 100;

typedef struct cmd
{
  bool execute_motor_instructions;
  Action_t action;
} CMD_t;

void timer2_init(void);
void debug(void);
void delay(int t);
void reset();
void print_sysinfo();
void parse(char, CMD_t *);
void update_sensor();

int main(void)
{
  // Configure Systick to be 100ms, 10HZ
  SysTick_Config(800000);

#ifdef USART
  usart_init();
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

  // Clear the screen
  usart_write_str(
      "\033[2J"
      "\033[0;0H"
      "Hello world! \r\n ");
  char c;

  CMD_t cmd;
  cmd.action = GO_FORWARD;
  cmd.execute_motor_instructions = false;

  while (1)
  {
    if (data_ready() > 0)
    {
      c = read_byte();
      parse(c, &cmd);
      if (cmd.execute_motor_instructions)
        motor_drive(pwm, duration, cmd.action);
      update_sensor();
      print_sysinfo();
    }
  }
}

void parse(char c, CMD_t *cmd)
{
  switch (c)
  {
  case 'r':
    reset();
    break; // never reach
  case 'a':
    cmd->action = GO_LEFT;
    cmd->execute_motor_instructions = true;
    break;
  case 'd':
    cmd->action = GO_RIGHT;
    cmd->execute_motor_instructions = true;
    break;
  case 'w':
    cmd->action = GO_FORWARD;
    cmd->execute_motor_instructions = true;
    break;
  case 's':
    cmd->action = GO_BACKWARD;
    cmd->execute_motor_instructions = true;
    break;
  case 'l':
    duration += d_step;
    cmd->execute_motor_instructions = false;
    break;
  case 'k':
    if( (int)(duration-d_step) >= 0)
      duration -= d_step;
    cmd->execute_motor_instructions = false;
    break;
  case 'o':
    if((pwm + p_step) <= 100)
      pwm += p_step;
    cmd->execute_motor_instructions = false;
    break;
  case 'i':
    if(pwm > 80)
      pwm -= p_step;
    cmd->execute_motor_instructions = false;
    break;
  default:
    break;
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
  if (tick == 0xFFFFFFFF)
    tick = 0;
  else
    tick++;

#ifdef GYRO
  dps = gyro_z();
  orientation += dps / 10;
#endif
}

// TODO: update HALL EFFECT
void update_sensor() {

#ifdef HCSR
      left = hcsr_distance(0);
      front = hcsr_distance(1);
      right = hcsr_distance(2);
#endif

}

void print_sysinfo()
{

  usart_write_str(
      "\033[2J"
      "\033[0;0H");
  usart_printf("tick: %d\r\n"
               "pwm: %d\r\n"
               "duration: %d\r\n"
               "dps: %d\r\n"
               "o: %d\r\n"
               "front: %d\r\n"
               "left: %d\r\n"
               "right: %d\r\n",
               tick,
               pwm,
               duration,
               dps,
               orientation,
               front,
               left,
               right);
}

/**
 * Initilize timer 2 to print out info.
 */
void timer2_init(void)
{

  RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;

  NVIC_SetPriority(TIM2_IRQn, 2);
  NVIC_EnableIRQ(TIM2_IRQn);

  TIM2->PSC = 4000 - 1;
  TIM2->ARR = 2000;
  TIM2->DIER |= TIM_DIER_UIE;
  TIM2->CR1 = TIM_CR1_CEN;
}

void TIM2_IRQHandler()
{
  print_sysinfo();
  TIM2->SR &= ~TIM_SR_UIF;
}

// Just a delay loop
void delay(int t)
{
  for (volatile int i = 0; i < t; i++)
    __NOP();
}

void debug()
{
  while (1)
    ;
}

void reset()
{
  NVIC_SystemReset();
}