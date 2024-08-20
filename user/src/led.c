#include "led.h"


/***********************************************
��������LED_Init
�������ܣ�led��ʼ��
�βΣ���
����ֵ����
��ע��LED1--PB11
			LED2--PB14
			LED3--PA11
***********************************************/
void LED_Init(void)
{
	//ʹ������ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB, ENABLE);

	//����GPIO
	//PB11
	GPIO_InitTypeDef GPIO_InitStruct;
	
	GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_11 | GPIO_Pin_14;
	GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOB, &GPIO_InitStruct);
	
	//PA11, PA14
	GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_11;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	
	//Ĭ�Ϲص�
	LED1_OFF;
	LED2_OFF;
	LED3_OFF;
}