#ifndef _RGBLED_H
#define _RGBLED_H

#include "stm32f10x.h"
#include "SysTick.h"
#include <stdbool.h>

#define DATA_PIN   GPIO_Pin_9   // 将数据线连接到指定引脚
#define CLK_PIN    GPIO_Pin_8   // 将时钟线连接到指定引脚

#define DATA_PORT  GPIOB        // GPIO 端口定义
#define CLK_PORT   GPIOB        // GPIO 端口定义

#define DATA_HIGH   (GPIO_SetBits(DATA_PORT, DATA_PIN))
#define DATA_LOW    (GPIO_ResetBits(DATA_PORT, DATA_PIN))

#define CLK_HIGH    (GPIO_SetBits(CLK_PORT, CLK_PIN))
#define CLK_LOW     (GPIO_ResetBits(CLK_PORT, CLK_PIN))

void RGBLED_Init(void);
void RGBLED_Show(uint8_t r, uint8_t g, uint8_t b);
void RGBLED_SendData(uint32_t data);
uint32_t RGBLED_EncodeColor(uint8_t r, uint8_t g, uint8_t b);
void NeonLightEffect(uint32_t delayTime);
void PoliceStrobeEffect(uint32_t delayTime);
void FireTruckWarningEffect(uint32_t delayTime);

#endif