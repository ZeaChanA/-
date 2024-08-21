#include "Step_Motor.h"

/**************************************************
*ʹ�ö�ʱ�����ж�����ȷ���Ʋ���������˶�����Ͳ���
***************************************************/

/*********************************
������:Stepmotor_Init
�������ܣ���������ĳ�ʼ��
�βΣ�
����ֵ��
��ע��
A   PA7
A_  PA6
B   PA5
B_  PA4
*********************************/
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



/*********************************
������:Step_Switch
�������ܣ�������������л�
�βΣ����Ʒ���Ĳ��� u8 direction
����ֵ��
��ע�����ݷ����л�����
*********************************/
void Step_Switch(u8 direction)
{
	static u8 step_counter=0;
	
	step_counter++;  
	
	if(direction == FORWARD)
	{
		//���򲽽�
		switch(step_counter)
		{
			case 1: STEP_1; break;//1000
			case 2: STEP_2; break;//0100
			case 3: STEP_3; break;//0010
			case 4: STEP_4; break;//0001
		}
	}
	
	if(direction == BACKWARD)
	{
		//���򲽽�
		switch(step_counter)
		{
			case 1: STEP_4; break;//0001
			case 2: STEP_3; break;//0010
			case 3: STEP_2; break;//0100
			case 4: STEP_1; break;//1000
		}
	}
	
	if(step_counter==4) step_counter=0;
}


/*********************************
������:Stepmotor_Stop
�������ܣ��������ֹͣ����
�βΣ���
����ֵ����
��ע��ֹͣ�����������
*********************************/
void Stepmotor_Stop(void)
{
	COIL_A_L; COIL_A_INV_L; COIL_B_L; COIL_B_INV_L;//0000
}



/*********************************
������:Stepmotor_Contrl
�������ܣ������������
�βΣ����Ʒ���;��� s32 steps
����ֵ����
��ע��
		���steps����0 ��ǰ�ƶ�
    ���stepsС��0 ����ƶ�
    ���steps����0 ֹͣ�ƶ�

���Ǿ���
*********************************/
u8 motor_direction = 0;
u32 motor_steps = 0;
void Stepmotor_Contrl(s32 steps)//typedef int32_t  s32;
{
	if(steps > 0)
	{
		motor_direction = FORWARD;
		motor_steps = steps;
		TIM_Cmd(TIM4,ENABLE);
	}
	
	if(steps < 0)
	{
		motor_direction = BACKWARD;
		motor_steps = -steps;
		TIM_Cmd(TIM4, ENABLE);
	}
	
	if(steps == 0)
	{
		Stepmotor_Stop();
		TIM_Cmd(TIM4, DISABLE);
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
	static u32 interrupt_count = 0;
	//1ms����һ��
	if(TIM_GetITStatus(TIM4,TIM_IT_Update) == SET)
	{
		//����жϹ���λ
		TIM_ClearITPendingBit(TIM4,TIM_IT_Update);
		interrupt_count++;    //��¼�����˶��ٴ��ж�
		
		Step_Switch(motor_direction);   //���ݷ���ִ�в����л�
		
		//��������
		if(interrupt_count >= motor_steps)
		{
			//�Ѿ�ִ�����
			interrupt_count = 0;
			Stepmotor_Stop();
      TIM_Cmd(TIM4,DISABLE);
		}
	}
}





