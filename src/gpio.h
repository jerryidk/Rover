#ifndef GPIO_H
#define GPIO_H

#include <stdint.h>
#include "stm32f072xb.h"

typedef struct GPIO_PIN{
    GPIO_TypeDef* gpio;
    uint8_t mode;
    uint8_t pin;
    uint8_t ospeed;
    uint8_t pupd;
    uint8_t otype;
    uint8_t af;
} GPIO_PIN_T;



#define LOW (0)
#define HIGH (1)
#define TOGGLE (2)

#define MODE_IN (0)
#define MODE_OUT (1)
#define MODE_AF (2)
// PUPD
#define PULL_UP (1)
#define PULL_DOWN (2)
// OTYPE
#define OTYPE_PP (0)
#define OTYPE_OD (1)
// OSPEED
#define OSPEED_LOW (0)
#define OSPEED_MED (1)
#define OSPEED_HIGH (2)


typedef uint8_t action_t;

uint8_t GPIO_init( GPIO_PIN_T* gpio_pin );
uint8_t GPIO_trigger( GPIO_PIN_T* gpio_pin, action_t action);
uint8_t GPIO_data_in( GPIO_PIN_T* gpio_pin );

#endif