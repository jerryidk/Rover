#include "usart.h"

extern uint32_t SystemClkFreq;

void usart3_init(uint32_t baud)
{
  RCC->APB1ENR |= RCC_APB1ENR_USART3EN;
  USART3->BRR = (uint32_t)(SystemClkFreq / baud);
  USART3->CR1 |= USART_CR1_RE | USART_CR1_TE | USART_CR1_UE;
}

void usart3_read(char *buf, uint8_t size)
{
  uint32_t i = 0;
  while (i < size)
  {
    if ((USART3->ISR & USART_ISR_RXNE) == USART_ISR_RXNE)
    {
      buf[i] = (char)(USART3->RDR);
      i++;
    }
  }
}

void usart3_write(char *buf)
{
  uint32_t i = 0;
  while (buf[i] != '\0')
  {
    if ((USART3->ISR & USART_ISR_TC) == USART_ISR_TC)
    {
      USART3->TDR = (uint8_t)buf[i];
      i++;
    }
  }
}

void usart3_write_num(uint16_t num)
{
  char buf[10];
  uint8_t len=10;

  for(int i=0; i<len; i++)
    buf[i] = '0';

  for (int i = 6; i >= 0 && num > 0; i--)
  {
    buf[i] = '0' + (num % 10);
    num = num / 10;
  }

  buf[7]='\r';
  buf[8]='\n';
  buf[9]='\0'; 
  usart3_write(buf);
}