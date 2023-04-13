#include "motor.h"

void motor_init()
{
    // Config a basic timer to measure time in ms
    RCC->APB1ENR |= RCC_APB1ENR_TIM6EN;
    TIM6->PSC = 8000 - 1;
    TIM6->ARR = 0xFFFF;
    TIM6->CNT = 0;

    motor_left_init();
    motor_right_init();
}

void motor_left_init()
{
    RCC->AHBENR |= RCC_AHBENR_GPIOCEN | RCC_AHBENR_GPIOAEN; 
    GPIOC->MODER |= GPIO_MODER_MODER3_0 |
                    GPIO_MODER_MODER4_0;

    GPIOC->ODR |= GPIO_ODR_3;
    GPIOC->ODR &= ~GPIO_ODR_4;

    RCC->APB1ENR |= RCC_APB1ENR_TIM14EN;

    // Timer 14, PWM 20khz
    // PA4 AF4
    GPIOA->MODER |= GPIO_MODER_MODER4_1;
    GPIOA->AFR[0] |= 0x4 << (16);

    TIM14->CCMR1 |= (TIM_CCMR1_OC1M_2 | TIM_CCMR1_OC1M_1 | TIM_CCMR1_OC1PE);
    TIM14->CCER |= TIM_CCER_CC1E; // Enable capture-compare channel 1
    TIM14->PSC = 3;
    TIM14->ARR = 100;          // PWM at 20kHz
    TIM14->CCR1 = 90;          // Start PWM at 0% duty cycle
    TIM14->CR1 |= TIM_CR1_CEN; // Enable timer
}

void motor_right_init(){

    RCC->AHBENR |= RCC_AHBENR_GPIOCEN | RCC_AHBENR_GPIOBEN;
    RCC->APB2ENR |= RCC_APB2ENR_TIM16EN; 

    // PC 5 and 6 
    GPIOC->MODER |= GPIO_MODER_MODER5_0 |
                    GPIO_MODER_MODER6_0;
    GPIOC->ODR |= GPIO_ODR_5;
    GPIOC->ODR &= ~GPIO_ODR_6;

    // Timer 16, PWM 20khz
    // PB 8 AF2
    GPIOB->MODER |= GPIO_MODER_MODER8_1;
    GPIOB->AFR[1] &= ~(0xF);
    GPIOB->AFR[1] |= (1 << 1);

    TIM16->PSC = 3;
    TIM16->ARR = 100;
    TIM16->CCR1 = 50;
    TIM16->CCMR1 |= TIM_CCMR1_OC1M_2 | TIM_CCMR1_OC1M_1 | TIM_CCMR1_OC1PE;
    TIM16->CCER |= TIM_CCER_CC1E;
    TIM16->BDTR |= TIM_BDTR_MOE; // Break or Dead ..
    TIM16->CR1 |= TIM_CR1_CEN;
    TIM16->EGR |= TIM_EGR_UG;
}

void motor_left_pwm(uint8_t pwm, MOTOR_ROT_DIRECTION_t direction)
{
    if (direction == ROT_FORWARD)
    {
        GPIOC->ODR |= GPIO_ODR_3;
        GPIOC->ODR &= ~GPIO_ODR_4;
    }
    else if (direction == ROT_BACKWARD)
    {
        GPIOC->ODR &= ~GPIO_ODR_3;
        GPIOC->ODR |= GPIO_ODR_4;
    }

    if (pwm <= 100)
        TIM14->CCR1 = ((uint32_t)pwm*TIM14->ARR)/100;
}
void motor_right_pwm(uint8_t pwm, MOTOR_ROT_DIRECTION_t direction)
{
    if (direction == ROT_FORWARD)
    {
        GPIOC->ODR |= GPIO_ODR_5;
        GPIOC->ODR &= ~GPIO_ODR_6;
    }
    else if (direction == ROT_BACKWARD)
    {
        GPIOC->ODR &= ~GPIO_ODR_5;
        GPIOC->ODR |= GPIO_ODR_6;
    }

    if (pwm <= 100)
        TIM16->CCR1 = ((uint32_t)pwm*TIM16->ARR)/100;
}

/* 
 * Interface from keyboard commands to motor control
 * speed    - duty cycle
 * duration - ms
*/
void motor_drive(uint8_t speed, uint32_t duration, Action_t action)
{
    MOTOR_ROT_DIRECTION_t left_dir;
    MOTOR_ROT_DIRECTION_t right_dir;
    uint8_t pwm;
    switch (action)
    {
        case GO_LEFT:
            pwm = (speed << 1); // divide by 2
            left_dir = ROT_FORWARD;
            right_dir = ROT_BACKWARD;
            break;

        case GO_RIGHT:
            pwm = (speed << 1); // divide by 2
            left_dir = ROT_BACKWARD;
            right_dir = ROT_FORWARD;
            break;

        case GO_FORWARD:
            pwm = speed;
            left_dir = ROT_FORWARD;
            right_dir = ROT_FORWARD;
            break;

        case GO_BACKWARD:
            pwm = speed;
            left_dir = ROT_BACKWARD;
            right_dir = ROT_BACKWARD;
            break;

        default :
            break;
    }

    motor_right_pwm(pwm, right_dir);
    motor_left_pwm(pwm, left_dir);
    uint32_t cnt = duration;
    while(cnt > 0)
    {
        delay_ms(1);
        cnt--;
    }
    
    // turn off
    motor_right_pwm(0, right_dir);
    motor_left_pwm(0, left_dir);
}

/**
 * TODO: Write PI controller to run the rover in given distance
 *
 * target_distance.
 */
void PI_control(int8_t target_speed)
{

}