#include "gpio.h"

uint8_t GPIO_init(GPIO_PIN_T *gpio_pin)
{
    GPIO_TypeDef *GPIO = gpio_pin->gpio;
    uint8_t pin = gpio_pin->pin;
    uint8_t mode = gpio_pin->mode;
    uint8_t pupd = gpio_pin->pupd;
    uint8_t ospeed = gpio_pin->ospeed;
    uint8_t otype = gpio_pin->otype;
    uint8_t af = gpio_pin->af;

    if (mode)
        GPIO->MODER |= (mode << (pin * 2));
    if (pupd)
        GPIO->PUPDR |= (pupd << (pin * 2));
    if (ospeed)
        GPIO->OSPEEDR |= (ospeed << (pin * 2));
    if (otype)
        GPIO->OTYPER |= (otype << (pin * 2));

    if (af)
    {
        if (pin <= 7)
            GPIOC->AFR[0] |= (af << (pin * 4));
        else
            GPIOC->AFR[1] |= (af << ((pin - 8) * 4));
    }

    return 1;
}

uint8_t GPIO_trigger(GPIO_PIN_T *gpio_pin, action_t action)
{

    if (gpio_pin->mode != MODE_OUT)
    {
        return 0;
    }

    GPIO_TypeDef *GPIO = gpio_pin->gpio;

    switch (action)
    {
    case HIGH:
        GPIO->ODR |= (1 << gpio_pin->pin);
        return 1;
    case LOW:
        GPIO->ODR &= ~(1 << gpio_pin->pin);
        return 1;
    case TOGGLE:
        GPIO->ODR ^= (1 << gpio_pin->pin);
        return 1;
    default:
        return 0;
    }
}

uint8_t GPIO_data_in(GPIO_PIN_T *gpio_pin)
{
    GPIO_TypeDef *GPIO = gpio_pin->gpio;
    uint8_t PIN = gpio_pin->pin;
    uint8_t MSK = 1 << PIN;
    return (uint8_t)((GPIO->IDR & MSK) >> PIN);
}