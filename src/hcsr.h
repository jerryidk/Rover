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
 *   Pin: 
 *   
*/
#include <stdint.h>
#include "timer.h"
#include "gpio.h"

typedef struct hcsr {
    BASIC_TIMER_T* timer; 
    GPIO_PIN_T* echo; 
    GPIO_PIN_T* trigger;
} HCSR_T;

extern uint32_t SystemClkFreq;


void     hcsr_init();
uint16_t hcsr_distance();
 

// check if configuration is correct
// uint8_t HCSR_check(HCSR_T* hcsr);

// return distance in mm
uint16_t HCSR_distance(HCSR_T* hcsr);
#endif

