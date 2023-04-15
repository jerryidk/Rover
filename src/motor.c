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
    TIM14->CCR1 = 50;          // pwm
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
    if (direction == ROT_BACKWARD)
    {
        GPIOC->ODR |= GPIO_ODR_3;
        GPIOC->ODR &= ~GPIO_ODR_4;
    }
    else if (direction == ROT_FORWARD)
    {
        GPIOC->ODR &= ~GPIO_ODR_3;
        GPIOC->ODR |= GPIO_ODR_4;
    }

    if (pwm <= 100)
        TIM14->CCR1 = ((uint32_t)pwm*TIM14->ARR)/100;
}

void motor_right_pwm(uint8_t pwm, MOTOR_ROT_DIRECTION_t direction)
{
    if (direction == ROT_BACKWARD)
    {
        GPIOC->ODR |= GPIO_ODR_5;
        GPIOC->ODR &= ~GPIO_ODR_6;
    }
    else if (direction == ROT_FORWARD)
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
void motor_drive(uint8_t pwm, uint32_t duration, Action_t action)
{
    MOTOR_ROT_DIRECTION_t left_motor_rot_dir;
    MOTOR_ROT_DIRECTION_t right_motor_rot_dir;
    switch (action)
    {
        case GO_LEFT:
            left_motor_rot_dir = ROT_BACKWARD;
            right_motor_rot_dir = ROT_FORWARD;
            break;

        case GO_RIGHT:
            left_motor_rot_dir = ROT_FORWARD;
            right_motor_rot_dir = ROT_BACKWARD;
            break;

        case GO_FORWARD:
            left_motor_rot_dir = ROT_FORWARD;
            right_motor_rot_dir = ROT_FORWARD;
            break;

        case GO_BACKWARD:
            left_motor_rot_dir = ROT_BACKWARD;
            right_motor_rot_dir = ROT_BACKWARD;
            break;
        default :
            break;
    }

    // Run 
    motor_right_pwm(pwm, right_motor_rot_dir);
    motor_left_pwm(pwm, left_motor_rot_dir);

    TIM6->CNT = 0;
    TIM6->CR1 |= TIM_CR1_CEN;
    // wait for duration ms
    while(TIM6->CNT < duration);
    

    // turn off
    motor_right_pwm(0, ROT_FORWARD);
    motor_left_pwm(0, ROT_FORWARD);
}
