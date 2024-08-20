#ifndef _DHT11_H
#define _DHT11_H

#include "stm32f10x.h"
#include "SysTick.h"

#define DHT11_L (GPIO_ResetBits(GPIOB, GPIO_Pin_3))
#define DHT11_H (GPIO_SetBits(GPIOB, GPIO_Pin_3))

void DHT11_Init(void);
void GPIO_SetOutput(void);
void GPIO_SetInput(void);
void DHT11_Start(void);
uint8_t DHT11_CheckResponse(void);
uint8_t DHT11_ReadByte(void);
void DHT11_ReadData(uint8_t *temperature, uint8_t *humidity);

#endif