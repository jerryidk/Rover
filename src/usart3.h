#ifndef USART3_H
#define USART3_H

/**
 *  USART perepheral driver
*/
#include <stdint.h> 
#include <stdlib.h>
#include "stm32f072xb.h"

#define USART3_TX (10)   
#define USART3_RX (11) 

// Init usart module
void usart3_init(uint32_t baud);

// Write 
void usart3_write_byte(char byte); 
void usart3_write(char* buf, uint8_t size);
uint8_t usart3_write_upto(char *buf, char terminator);
void usart3_write_num(int16_t num);
void usart3_write_str(char* str);

// Read
char usart3_read_byte(); 
void usart3_read(char *buf, uint8_t size);
uint8_t usart3_read_upto(char *buf, char terminator); 
#endif