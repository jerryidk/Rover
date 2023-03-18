#include "usart3.h"

extern uint32_t SystemClkFreq;

void usart3_init(uint32_t baud)
{
  USART3->BRR = (uint32_t)(SystemClkFreq / baud);
  USART3->CR1 |= USART_CR1_RE | USART_CR1_TE | USART_CR1_UE;
}

void usart3_write_byte(char byte) {
  while((USART3->ISR & USART_ISR_TXE) != USART_ISR_TXE);
  USART3->TDR = (uint8_t)byte;
  while((USART3->ISR & USART_ISR_TC) == USART_ISR_TC);
}

void usart3_write(char *buf, uint8_t size)
{
  uint8_t i = 0;
  while(i<size){
    usart3_write_byte(buf[i]);
    i++;
  }
}

uint8_t usart3_write_upto(char *buf, char terminator)
{
  uint8_t i = 0;
  char byte; 
  while(1){
    byte = buf[i];
    usart3_write_byte(byte);
    i++;
    if(byte == terminator)
      return i;
  }

}

// TODO
void usart3_write_num(int32_t num)
{
  char buf[32];
  uint8_t len = 0; 

  if(num == 10)
    usart3_write("10\r\n", 4);

  uint8_t sign = num >> 31;
  if(sign == 1){
    buf[0] = '-';
    len++;
  }

  num = (int32_t) abs(num);
  
  while(num){
    buf[len] = num%10 + '0';
    len++;
    num = num/10;

  }

  buf[len]   = '\r';
  buf[len+1] = '\n'; 
  len += 2;
  usart3_write(buf, len);
}

char usart3_read_byte() {
  while ((USART3->ISR & USART_ISR_RXNE) != USART_ISR_RXNE);
  return (char)(USART3->RDR);
}

void usart3_read(char *buf, uint8_t size)
{
  uint8_t i=0;
  while(i<size){
    buf[i] = usart3_read_byte();
    i++;
  }
}

uint8_t usart3_read_upto(char *buf, char terminator)
{
  uint8_t i=0;
  char byte;
  while(1){
    byte = usart3_read_byte();
    buf[i] = byte;
    i++;
    if(byte == terminator)
      return i;
  }
}

