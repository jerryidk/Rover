#include "hcsr.h"


uint8_t HCSR_check(HCSR_T* hcsr){

    if(hcsr->timer->psc != (SystemClkFreq/1000000) - 1) 
        return 0;

    if(hcsr->timer->arr < 10000) 
        return 0;

    if(hcsr->echo->mode != MODE_IN)
        return 0;
    
    if(hcsr->trigger->mode != MODE_OUT)
        return 0;

    return 1;
}


// return distance in mm
uint16_t HCSR_distance(HCSR_T* hcsr){

    BASIC_TIMER_T* tim = hcsr->timer;
    GPIO_PIN_T*    echo = hcsr->echo;
    GPIO_PIN_T*    trigger = hcsr->trigger;

    // Send trigger signal
    GPIO_trigger(trigger, ON); 
    Timer_delay(tim, 15); 
    GPIO_trigger(trigger, OFF); 
    // wait for echo line to turn on
    while(GPIO_data_in(echo) != 1); 
    // start timer
    Timer_start(tim);
    // wait for echo line to turn off
    while(GPIO_data_in(echo) != 0);
    // get timer passed  
    uint16_t t = Timer_getCnt(tim); // in us
    // terminate timer
    Timer_end(tim);
    return (uint16_t)((t) / 58); // d  = v 340 m/s / t 
}