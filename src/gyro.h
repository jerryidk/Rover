#ifndef GYRO_H
#define GYRO_H

#include <stdint.h>
#include "stm32f072xb.h"

#define WHOAMI (0x0F)
#define CTRL1 (0x20)
#define ST (0x27)
#define OUT_X_L (0x28)
#define OUT_X_H (0x29)
#define OUT_Y_L (0x2A)
#define OUT_Y_H (0x2B)
#define OUT_Z_L (0x2C)
#define OUT_Z_H (0x2D)
#define CS_ENABLE (GPIOC->BSRR |= GPIO_BSRR_BR_0)
#define CS_DISABLE (GPIOC->BSRR |= GPIO_BSRR_BS_0)
#define R (0x1)
#define W (0x0)
/**
 * Description:
 *
 * PC0 - CS enable
 * PB13 - SCK
 * PB14 - MISO
 * PB15 - MOSI
 */
void Gyro_init();
void Gyro_enable();
int16_t Gyro_x();
int16_t Gyro_y();
int16_t Gyro_z();
uint8_t Gyro_send(uint8_t rw, uint8_t reg, uint8_t data);
#endif