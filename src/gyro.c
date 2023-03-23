#include "gyro.h"

void Gyro_init()
{
    RCC->AHBENR |= RCC_AHBENR_GPIOCEN | RCC_AHBENR_GPIOBEN;
    // PC0 MODE OUT
    GPIOC->MODER |= GPIO_MODER_MODER0_0;
    // PB 13-15 MODE AF 0 ( SPI2 )
    GPIOB->MODER |= GPIO_MODER_MODER13_1 | GPIO_MODER_MODER14_1 | GPIO_MODER_MODER15_1;
  
    // SPI
    RCC->APB1ENR |= RCC_APB1ENR_SPI2EN;
    // f_plck / 16
    SPI2->CR1 |= SPI_CR1_MSTR | SPI_CR1_BR_1 | SPI_CR1_BR_0 | SPI_CR1_SSM;
    // 16bit transfer
    SPI2->CR2 |= SPI_CR2_SSOE | SPI_CR2_DS_0 | SPI_CR2_DS_1 | SPI_CR2_DS_2 | SPI_CR2_DS_3;
    // enable
    SPI2->CR1 |= SPI_CR1_SPE;

}

uint8_t Gyro_send(uint8_t rw, uint8_t reg, uint8_t data)
{

    // enable CS
    CS_ENABLE;

    while ((SPI2->SR & SPI_SR_TXE) != SPI_SR_TXE)
        ;
    // MSB
    SPI2->DR = (uint16_t) ((rw << 15) | ((reg&0x3F)<< 8) | data);

    uint8_t reply; 
    while ((SPI2->SR & SPI_SR_RXNE) != SPI_SR_RXNE)
        ;
    
    reply = (uint8_t) (SPI2->DR & 0xFF);
    // disable cs
    CS_DISABLE;

    return reply;
}

void Gyro_enable()
{
    Gyro_send(W, CTRL1, 0x0F);
}

int16_t Gyro_x()
{
    int16_t H = (int16_t) (Gyro_send(R, OUT_X_H, 0x0) << 8);
    int16_t L = (int16_t) (Gyro_send(R, OUT_X_L, 0x0));
    return H+L;
}

int16_t Gyro_y()
{
    int16_t H = (int16_t) (Gyro_send(R, OUT_Y_H, 0x0) << 8);
    int16_t L = (int16_t) (Gyro_send(R, OUT_Y_L, 0x0));
    return H+L;
}

int16_t Gyro_z()
{
    int16_t H = (int16_t) (Gyro_send(R, OUT_Z_H, 0x0) << 8);
    int16_t L = (int16_t) (Gyro_send(R, OUT_Z_L, 0x0));
    return H+L;
}