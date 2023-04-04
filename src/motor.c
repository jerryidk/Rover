
#include "motor.h"

void motor_init() {

    // Motor 1 pins
    
    motor1_init();

    // Motor 2 pins
    /*
    GPIOA->MODER |= GPIO_MODER_MODER2_0 | GPIO_MODER_MODER3_0;
    GPIOC->MODER |= GPIO_MODER_MODER6_0;
    */
}


void motor1_init() {

    // PA0 and 1 to OUT
    GPIOA->MODER |= GPIO_MODER_MODER0_0 | GPIO_MODER_MODER1_0;
    // PB10 to AF2, PB1 to AF0
    GPIOB->MODER |= GPIO_MODER_MODER10_1 | GPIO_MODER_MODER0_1;
    GPIOB->AFR[1] |= 0x2 << GPIO_AFRH_AFSEL10_Pos; 

    // Configure Timer 2 and Timer 14
    
    // Timer 2 is Encoder mode, Timer 14 is PWM mode
}

void encoder_init() 
{

}
