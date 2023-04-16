#include "usart.h"

#define BUFFER_MAX (128)
char buffer[BUFFER_MAX];
uint8_t buffer_size = 0; // how many data on buffer
uint8_t read_so_far = 0; // read counter

void flush_buf()
{
  for (int i = 0; i < BUFFER_MAX; i++)
    buffer[i] = 0;
}

void dump_info() 
{
  usart_printf("bz: %d \r\n", buffer_size);
  usart_printf("rs: %d \r\n", read_so_far);
  for(int i=0; i<buffer_size; i++)
    usart_write_byte(buffer[i]);

  usart_write_str("\r\n");

}

char read_byte(){

  char byte = buffer[read_so_far];
  read_so_far++;
  return byte;
}

uint8_t data_ready()
{
  return buffer_size - read_so_far;
}

void USART3_4_IRQHandler(void)
{

  // Should never Overflow
  if ((USART3->ISR & USART_ISR_ORE) == USART_ISR_ORE)
  {
    USART3->ICR |= USART_ICR_ORECF;
    return;
  }

  if(read_so_far == buffer_size)
  {
    buffer_size = 0;
    read_so_far = 0;
  }

  // Clear the buffer when application has read all data
  if (buffer_size == BUFFER_MAX){
    flush_buf();
    buffer_size = 0;
    read_so_far = 0;
  }
    
  // Only read when buffer is not full.
  if ((USART3->ISR & USART_ISR_RXNE) == USART_ISR_RXNE &&
      buffer_size < BUFFER_MAX)
  {
    buffer[buffer_size] = (char)USART3->RDR;
    buffer_size++;
    return;
  }

  // Ignore when buffer is full, but still clears.
  USART3->RQR |= USART_RQR_RXFRQ; 
}

void usart_init()
{
  RCC->AHBENR |= RCC_AHBENR_GPIOCEN;
  RCC->APB1ENR |= RCC_APB1ENR_USART3EN;

  // PC10, PC11
  GPIOC->MODER |= GPIO_MODER_MODER10_1 | GPIO_MODER_MODER11_1;
  // AF1, AF1
  GPIOC->AFR[1] |= (0x1 << GPIO_AFRH_AFSEL10_Pos) |
                   (0x1 << GPIO_AFRH_AFSEL11_Pos);

  NVIC_EnableIRQ(USART3_4_IRQn);
  NVIC_SetPriority(USART3_4_IRQn, 1);
  // USART 3
  USART3->BRR = (int) 8000000/9600;
  USART3->CR1 = USART_CR1_RXNEIE | USART_CR1_RE | USART_CR1_TE | USART_CR1_UE;
}

void usart_printf(const char *format, ...)
{
  va_list args;
  va_start(args, format);
  while (*format)
  {
    if (*format == '%')
    {
      switch (*(++format))
      {
      case 'd':
      {
        int32_t arg = va_arg(args, int32_t);
        usart_write_num(arg);
        break;
      }
      case 's':
      {
        char *arg = va_arg(args, char *);
        usart_write_str(arg);
        break;
      }
      default:
        usart_write_byte(*format);
      }
    }
    else
    {
      usart_write_byte(*format);
    }
    ++format;
  }
  va_end(args);
}

void usart_write_byte(char byte)
{
  while ((USART3->ISR & USART_ISR_TXE) != USART_ISR_TXE)
    ;
  USART3->TDR = (uint8_t)byte;
  while ((USART3->ISR & USART_ISR_TC) == USART_ISR_TC)
    ;
}

void usart_write(char *buf, uint8_t size)
{
  uint8_t i = 0;
  while (i < size)
  {
    usart_write_byte(buf[i]);
    i++;
  }
}

uint8_t usart_write_upto(char *buf, char terminator)
{
  uint8_t i = 0;
  char byte;
  while (1)
  {
    byte = buf[i];
    usart_write_byte(byte);
    i++;
    if (byte == terminator)
      return i;
  }
}

void usart_write_str(char *str)
{

  int i = 0;
  while (str[i] != '\0')
  {
    usart_write_byte(str[i]);
    i++;
  }
}

void usart_write_num(int num)
{
  char buf[32];
  itoa(num, buf, 10); // base 10.
  usart_write_str(buf);
}

char usart_read_byte()
{
  while (1)
  {

    if ((USART3->ISR & USART_ISR_ORE) == USART_ISR_ORE)
    {
      USART3->ICR |= USART_ICR_ORECF;
      return 'E';
    }

    if ((USART3->ISR & USART_ISR_RXNE) == USART_ISR_RXNE)
      return (char)(USART3->RDR);
  }
}

void usart_read(char *buf, uint8_t size)
{
  uint8_t i = 0;
  while (i < size)
  {
    buf[i] = usart_read_byte();
    i++;
  }
}

uint8_t usart_read_upto(char *buf, char terminator)
{
  uint8_t i = 0;
  char byte;
  while (1)
  {
    byte = usart_read_byte();
    buf[i] = byte;
    i++;
    if (byte == terminator)
      return i;
  }
}
