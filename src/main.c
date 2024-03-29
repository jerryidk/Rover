#include "main.h"

// System
volatile uint32_t tick = 0; // 1 tick = 100 ms
volatile int16_t dps = 0;
volatile int16_t orientation = 0; // degree
// Motor
volatile uint8_t pwm = 100;
volatile uint32_t duration = 630; // ms
volatile uint8_t p_step = 1;
volatile uint8_t d_step = 50;
// Hcsr
volatile uint16_t front = 0xFFFF; // cm
volatile uint16_t left = 0xFFFF;
volatile uint16_t right = 0xFFFF;

// PI
extern volatile uint32_t target_distance; // cm

// Hall
extern volatile uint32_t left_cnt;
extern volatile uint32_t right_cnt;

void debug(void);
void delay(int t);
void reset(void);
void execute(char);
void update_distance();
void autorun(void);

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

#ifdef HALL
  hall_init();
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

  while (1)
  {
    if (data_ready() > 0)
    {

      c = read_byte();

      // clear screen
      usart_write_str(
          "\033[2J"
          "\033[0;0H");
      usart_write_str("Start\r\n");
      execute(c);
      usart_write_str("Finish\r\n");
      update_distance();
      usart_printf("---- Sys ----\r\n"
                   "tick: %d\r\n"
                   "---- Motor ----\r\n"
                   "pwm: %d\r\n"
                   "duration: %d\r\n"
                   "---- Gyro ----\r\n"
                   "angular velo: %d\r\n"
                   "orientation: %d\r\n"
                   "----- PI ----\r\n"
                   "target_distance: %d\r\n"
                   "-----HCSR----\r\n"
                   "F: %d\r\n"
                   "L: %d\r\n"
                   "R: %d\r\n",
                   tick,
                   pwm,
                   duration,
                   dps,
                   orientation,
                   target_distance,
                   front,
                   left,
                   right);
    }
  }
}

void execute(char c)
{
  switch (c)
  {
  case 'r':
    reset();
    break; // never reach
  case 'a':
    motor_drive(pwm, duration, GO_LEFT);
    break;
  case 'd':
    motor_drive(pwm, duration, GO_RIGHT);
    break;
  case 'w':
    motor_drive(pwm, duration, GO_FORWARD);
    break;
  case 's':
    motor_drive(pwm, duration, GO_BACKWARD);
    break;
  case 'l':
    duration += d_step;
    break;
  case 'k':
    if ((int)(duration - d_step) >= 0)
      duration -= d_step;
    break;
  case 'o':
    if ((pwm + p_step) <= 100)
      pwm += p_step;
    break;
  case 'i':
    if (pwm > 80)
      pwm -= p_step;
    break;
  case 'p':
    PI_init();
    break;
  case 'v':
    motor_drive(pwm, target_distance * 3000 / 110, GO_FORWARD);
    break;
  case '+':
    target_distance += 100;
    break;
  case '-':
    target_distance -= 100;
    break;
  case 'm':
    autorun();
    break;
  default:
    break;
  }
}

/*
 * Input: Hcsr sensor
 * Output action
 */
void autorun(void)
{

  const uint32_t threshold = 15;
  const uint32_t turn = 630;
  const uint32_t dist = 300;

  uint32_t old_leftcnt = left_cnt;
  uint32_t old_rightcnt = right_cnt;
  uint32_t stationary = 0;
  while (1)
  {

    if (stationary >= 5)
    {
      usart_printf("Stuck \r\n");
      break;
    }

    if (data_ready() > 0)
    {
      usart_printf("Stopping \r\n");
      break;
    }

    if (left_cnt == old_leftcnt &&
        right_cnt == old_rightcnt)
    {
      stationary++;
    }
    else
    {
      old_leftcnt = left_cnt;
      old_rightcnt = right_cnt;
      stationary = 0;
    }
    
    update_distance();

    if (front > threshold)
    {
      motor_drive(pwm, dist, GO_FORWARD);
      continue;
    }

    if (front < threshold && left > threshold && right < threshold)
    {
      motor_drive(pwm, turn, GO_LEFT);
      continue;
    }

    if (front < threshold && right > threshold && left < threshold)
    {
      motor_drive(pwm, turn, GO_RIGHT);
      continue;
    }

    if (front < threshold && left > threshold && right > threshold)
    {
      // if face left
      if (orientation < 0)
        motor_drive(pwm, turn, GO_RIGHT);
      else
        motor_drive(pwm, turn, GO_LEFT);

      continue;
    }

    if (front < threshold && left < threshold && right < threshold)
    {
      motor_drive(pwm, turn * 2, GO_LEFT);
      usart_printf("Trapped \r\n");
      return;
    }
  }

  motor_drive(0, 0, GO_FORWARD);
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
  orientation = orientation % 360;
#endif
}

void update_distance()
{
  // We can disable IRQ because Rover is not moving.
  __disable_irq();
#ifdef HCSR
  left = hcsr_distance(0);
  front = hcsr_distance(1);
  right = hcsr_distance(2);
#endif
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
  while (1)
    ;
}

void reset()
{
  NVIC_SystemReset();
}