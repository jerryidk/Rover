
/**
 *  HC-SR Ultrasonic Sensor
 *  
 *  Blocking operation API. 
 *  
 *  v = 360m/s 
 *  d = v * t / 2 = 0.017 cm/us * t = t / 59 
 *  t_max = d_max / v    
 *  
 *  t_max = 1000 us
 *  d_max = 17 cm
 *   
 * 
 *  4 pins; 
 *      5v power
 *      trigger
 *      echo
 *      GND
 *   
 *  Pin: 
 *   
 * | hcsr1      | PB3  | IN   | ECHO    |  
 * | hcsr1      | PA5  | OUT  | TRIGER  | 
 * | hcsr2      | PB4  | IN   | ECHO    | 
 * | hcsr2      | PA6  | OUT  | TRIGER  | 
 * | hcsr3      | PB5  | IN   | ECHO    |
 * | hcsr3      | PA7  | OUT  | TRIGER  |
 * 
 * 
*/

#ifndef HCSR_H
#define HCSR_H

#include "stm32f072xb.h"

void     hcsr_init();
// Return distacne in cm, arg indicates which hcsr sensor (0 to 2). 
uint16_t hcsr_distance(uint8_t offset);
#endif

