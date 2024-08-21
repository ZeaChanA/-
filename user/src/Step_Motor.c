#include "Step_Motor.h"


/*
������:Stepmotor_Init
�������ܣ���������ĳ�ʼ��
�βΣ�
����ֵ��
��ע��
A   PA7
A_  PA6
B   PA5
B_  PA4
*/
void Stepmotor_Init(void)
{
	//����������ſ���
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	GPIO_InitTypeDef gpio_InitTypeDef;
	gpio_InitTypeDef.GPIO_Pin=GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;
	gpio_InitTypeDef.GPIO_Speed=GPIO_Speed_2MHz;
	gpio_InitTypeDef.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_Init(GPIOA,&gpio_InitTypeDef);
	
	
	//��ʱ��  ����һ���ж�
  //��ʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);
	
	//ʱ������
	TIM_TimeBaseInitTypeDef tim_TimeBaseInitTypeDef;
	tim_TimeBaseInitTypeDef.TIM_Prescaler=72-1;
	tim_TimeBaseInitTypeDef.TIM_Period=1000-1;
	tim_TimeBaseInitTypeDef.TIM_CounterMode=TIM_CounterMode_Up;
	tim_TimeBaseInitTypeDef.TIM_ClockDivision=TIM_CKD_DIV1;
	TIM_TimeBaseInit(TIM4,&tim_TimeBaseInitTypeDef);
	//CR1��������
	TIM_ARRPreloadConfig(TIM4,ENABLE);        //ʹ��ARRӰ�ӼĴ���
	TIM_UpdateDisableConfig(TIM4,DISABLE);    //���½�ֹ
	TIM_UpdateRequestConfig(TIM4,TIM_UpdateSource_Regular);  //��������Դ
	TIM_SelectOnePulseMode(TIM4,TIM_OPMode_Repetitive);      //�Ƿ��ظ�ִ�м�����
	//��Ϊ���������¼�
	TIM_GenerateEvent(TIM4,TIM_EventSource_Update);
	TIM_ClearFlag(TIM4,TIM_FLAG_Update);
	
	
	//�ж�����
	TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE);  //ʹ�ܸ����ж�
	
	NVIC_InitTypeDef nvic_InitTypeDef;
	nvic_InitTypeDef.NVIC_IRQChannel=TIM4_IRQn;
	nvic_InitTypeDef.NVIC_IRQChannelPreemptionPriority=3;
	nvic_InitTypeDef.NVIC_IRQChannelSubPriority=1;
	nvic_InitTypeDef.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&nvic_InitTypeDef);
	
	//��ʱ�رռ�����   ���ƺ����п��ƴ�
	TIM_Cmd(TIM4,DISABLE);
	
	//ÿ�ο����ص������
	Stepmotor_Contrl(-800);
}



/*
������:Beat_Ch
�������ܣ������л�����
�βΣ����Ʒ���Ĳ��� u8 dir
����ֵ��
��ע��
*/
void Beat_Ch(u8 dir)
{
	static u8 cnt=0;
	
	cnt++;  
	
	if(dir == TURN)
	{
		//����1~4�����л�
		switch(cnt)
		{
			case 1:BEAT1;break;
			case 2:BEAT2;break;
			case 3:BEAT3;break;
			case 4:BEAT4;break;
		}
	}
	
	if(dir == REVERSE)
	{
		//����4~1�Ľ����л�
		switch(cnt)
		{
			case 1:BEAT4;break;
			case 2:BEAT3;break;
			case 3:BEAT2;break;
			case 4:BEAT1;break;
		}
	}
	
	if(cnt==4) cnt=0;
}


/*
������:Stemp_Stop
�������ܣ��������ֹͣ����
�βΣ�void
����ֵ��
��ע��
*/
void Stemp_Stop(void)
{
	A_L;_A_L;B_L;_B_L;
}



/*
������:Stepmotor_Contrl
�������ܣ�����������ƺ���
�βΣ����Ʒ���Ӿ���  �������Ʒ���  ��С���ƾ���
s32 light
����ֵ��
��ע��
�������0  �����ƶ�
���С��0  �����ƶ�
�������0  ���ƶ�

light---�Խ���Ϊ��λ

���Ǿ���
*/
u8 dir=0;
u32 dis=0;
void Stepmotor_Contrl(s32 light)//typedef int32_t  s32;
{
	if(light>0)
	{
		dir=TURN;
		dis=light;
		TIM_Cmd(TIM4,ENABLE);
	}
	
	if(light<0)
	{
		dir=REVERSE;
		dis=-light;
		TIM_Cmd(TIM4,ENABLE);
	}
	
	if(light == 0)
	{
		Stemp_Stop();
		TIM_Cmd(TIM4,DISABLE);
	}
}



/*
������:TIM4_IRQHandler
�������ܣ���ʱ��2���жϷ�����
�βΣ�
����ֵ��
��ע��
*/
void TIM4_IRQHandler(void)
{
	static u32 n=0;
	//1ms����һ��
	if(TIM_GetITStatus(TIM4,TIM_IT_Update)==SET)
	{
		//����жϹ���λ
		TIM_ClearITPendingBit(TIM4,TIM_IT_Update);
		n++;    //��¼�����˶��ٴ��ж�
		
		Beat_Ch(dir);   //�����л�
		
		//��������
		if(n>=dis)
		{
			//�Ѿ�ִ�����
			n=0;
			Stemp_Stop();
      TIM_Cmd(TIM4,DISABLE);
		}
	}
}





