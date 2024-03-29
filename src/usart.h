#ifndef USART3_H
#define USART3_H

/**
 *  USART perepheral driver
*/
#include <stdint.h> 
#include <stdlib.h>
#include <stdarg.h>
#include "stm32f072xb.h"


uint8_t data_ready(void);
char read_byte(void); 
void dump_info();

void usart_init();
void usart_printf(const char *format, ...);

// Write 
void usart_write_byte(char byte); 
void usart_write(char* buf, uint8_t size);
uint8_t usart_write_upto(char *buf, char terminator);
void usart_write_num(int num);
void usart_write_str(char* str);

// Read
char usart_read_byte(); 
void usart_read(char *buf, uint8_t size);
uint8_t usart_read_upto(char *buf, char terminator); 
#endif
