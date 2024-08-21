#ifndef _STEPMOTOR_H
#define _STEPMOTOR_H

#include "stm32f10x.h"

//引脚状态宏定义
//coil 绕组(线圈)
#define COIL_A_H      GPIO_SetBits(GPIOA, GPIO_Pin_7)
#define COIL_A_L      GPIO_ResetBits(GPIOA, GPIO_Pin_7)

#define COIL_A_INV_H  GPIO_SetBits(GPIOA, GPIO_Pin_6)
#define COIL_A_INV_L  GPIO_ResetBits(GPIOA, GPIO_Pin_6)

#define COIL_B_H      GPIO_SetBits(GPIOA, GPIO_Pin_5)
#define COIL_B_L      GPIO_ResetBits(GPIOA, GPIO_Pin_5)

#define COIL_B_INV_H  GPIO_SetBits(GPIOA, GPIO_Pin_4)
#define COIL_B_INV_L  GPIO_ResetBits(GPIOA, GPIO_Pin_4)

// 步进电机步进宏定义
#define STEP_1  {COIL_A_H; COIL_A_INV_L; COIL_B_L; COIL_B_INV_H;} // 1000
#define STEP_2  {COIL_A_L; COIL_A_INV_H; COIL_B_L; COIL_B_INV_H;} // 0100
#define STEP_3  {COIL_A_L; COIL_A_INV_H; COIL_B_H; COIL_B_INV_L;} // 0010
#define STEP_4  {COIL_A_H; COIL_A_INV_L; COIL_B_H; COIL_B_INV_L;} // 0001


// 方向定义
#define FORWARD    0   // 正转
#define BACKWARD   1   // 反转

void Stepmotor_Init(void);
void Step_Switch(u8 direction);
void Stepmotor_Stop(void);
void Stepmotor_Contrl(s32 steps);


#endif

