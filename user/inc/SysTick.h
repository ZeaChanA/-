#ifndef SYSTICK_H
#define SYSTICK_H

#include "stm32f10x.h"

extern volatile uint32_t uwTick;

void delay_us(uint32_t us);
void delay_ms(uint32_t ms);
void SysTick_Init(uint32_t ticks);

#endif