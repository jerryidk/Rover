#include "gpio.h"


uint8_t GPIO_init(GPIO_PIN_T* gpio_pin)
{
    GPIO_TypeDef* GPIO = gpio_pin->gpio;
    

    uint8_t pin = gpio_pin->pin;
    switch (gpio_pin->mode)
    {
    case MODE_IN:
        GPIO->MODER &= ~(1 << (pin * 2));
        GPIO->PUPDR |= (gpio_pin->pupd << (pin*2)); 
        return 1;
    case MODE_OUT:
        GPIO->MODER |= (MODE_OUT << (pin * 2));
        GPIO->OSPEEDR |= (gpio_pin->ospeed << (pin * 2));
        GPIO->OTYPER |= (gpio_pin->otype << (pin * 2));
        GPIO->PUPDR |= (gpio_pin->pupd << (pin*2)); 
        return 1;
    case MODE_AF:
        GPIO->MODER |= (MODE_AF << (pin * 2));
        if (pin <= 7)
            GPIOC->AFR[0] |= (gpio_pin->af << (pin * 4));
        else
            GPIOC->AFR[1] |= (gpio_pin->af << ((pin - 8) * 4));
        return 1;
    default:
        return 0;
    }
}

uint8_t GPIO_trigger(GPIO_PIN_T * gpio_pin, action_t action) {

    if(gpio_pin->mode != MODE_OUT){
        return 0;
    }

    GPIO_TypeDef *GPIO = gpio_pin->gpio;

    switch (action)
    {
    case ON:
        GPIO->ODR |= (1 << gpio_pin->pin); 
        return 1;
    case OFF:
        GPIO->ODR &= ~(1 << gpio_pin->pin); 
        return 1; 
    case TOGGLE:
        GPIO->ODR ^= (1 << gpio_pin->pin); 
        return 1;
    default:
        return 0;
    }
}


uint8_t GPIO_data_in(GPIO_PIN_T *gpio_pin) {
    GPIO_TypeDef * GPIO = gpio_pin->gpio; 
    uint8_t PIN = gpio_pin->pin; 
    uint8_t MSK = 1 << PIN; 
    return (uint8_t) ((GPIO->IDR & MSK) >> PIN);
}