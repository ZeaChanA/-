#ifndef _STEPMOTOR_H
#define _STEPMOTOR_H

#include "stm32f10x.h"

#define A_H   GPIO_SetBits(GPIOA,GPIO_Pin_7)
#define A_L   GPIO_ResetBits(GPIOA,GPIO_Pin_7)

#define _A_H  GPIO_SetBits(GPIOA,GPIO_Pin_6)
#define _A_L  GPIO_ResetBits(GPIOA,GPIO_Pin_6)

#define B_H   GPIO_SetBits(GPIOA,GPIO_Pin_5)
#define B_L   GPIO_ResetBits(GPIOA,GPIO_Pin_5)

#define _B_H  GPIO_SetBits(GPIOA,GPIO_Pin_4)
#define _B_L  GPIO_ResetBits(GPIOA,GPIO_Pin_4)

#define BEAT1  {A_H;_A_L;B_L;_B_H;}//1000
#define BEAT2  {A_L;_A_H;B_L;_B_H;}//0100
#define BEAT3  {A_L;_A_H;B_H;_B_L;}//0010
#define BEAT4  {A_H;_A_L;B_H;_B_L;}//0001


#define TURN    0   // 正转
#define REVERSE 1   // 反转

void Stepmotor_Init(void);
void Beat_Ch(u8 dir);
void Stepmotor_Contrl(s32 light);


#endif

