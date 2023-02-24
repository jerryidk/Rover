#ifndef USART3_H
#define USART3_H

/**
 *  USART perepheral driver
*/
#include <stdint.h> 
#include "stm32f072xb.h"

void usart3_init(uint32_t baud);
void usart3_write(char *buf);
void usart3_read(char *buf, uint8_t size);
void usart3_write_num(uint16_t num);
#endif