#ifndef HCSR_H
#define HCSR_H


/**
 *  HC-SR Ultrasonic Sensor
 *  
 *  4 pins; 
 *      5v power
 *      trigger
 *      echo
 *      GND
 * 
*/
#include <stdint.h>
#include "timer.h"
#include "gpio.h"
#include "usart.h"

typedef struct hcsr {
    BASIC_TIMER_T* timer; 
    GPIO_PIN_T* echo; 
    GPIO_PIN_T* trigger;
} HCSR_T;

extern uint32_t SystemClkFreq;
// check if configuration is correct
uint8_t HCSR_check(HCSR_T* hcsr);

// return distance in mm
uint16_t HCSR_distance(HCSR_T* hcsr);
#endif

