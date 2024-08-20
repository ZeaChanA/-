#ifndef LED_H
#define LED_H

#include "stm32f10x.h"

#define LED1_ON (GPIO_SetBits(GPIOB, GPIO_Pin_11))
#define LED1_OFF (GPIO_ResetBits(GPIOB, GPIO_Pin_11))
#define LED1_TOGGLE (GPIO_WriteBit(GPIOB, GPIO_Pin_11, (BitAction)(1 - GPIO_ReadOutputDataBit(GPIOB, GPIO_Pin_11))))

#define LED2_ON (GPIO_SetBits(GPIOB, GPIO_Pin_14))
#define LED2_OFF (GPIO_ResetBits(GPIOB, GPIO_Pin_14))
#define LED2_TOGGLE (GPIO_WriteBit(GPIOB, GPIO_Pin_14, (BitAction)(1 - GPIO_ReadOutputDataBit(GPIOB, GPIO_Pin_14))))

#define LED3_ON (GPIO_SetBits(GPIOA, GPIO_Pin_11))
#define LED3_OFF (GPIO_ResetBits(GPIOA, GPIO_Pin_11))
#define LED3_TOGGLE (GPIO_WriteBit(GPIOA, GPIO_Pin_11, (BitAction)(1 - GPIO_ReadOutputDataBit(GPIOA, GPIO_Pin_11))))

void LED_Init(void);

#endif