#include "Step_Motor.h"

/**************************************************
*使用定时器和中断来精确控制步进电机的运动方向和步数
***************************************************/

/*********************************
函数名:Stepmotor_Init
函数功能：步进电机的初始化
形参：
返回值：
备注：
A   PA7
A_  PA6
B   PA5
B_  PA4
*********************************/
void Stepmotor_Init(void)
{
	//步进电机引脚控制
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	GPIO_InitTypeDef gpio_InitTypeDef;
	gpio_InitTypeDef.GPIO_Pin=GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;
	gpio_InitTypeDef.GPIO_Speed=GPIO_Speed_2MHz;
	gpio_InitTypeDef.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_Init(GPIOA,&gpio_InitTypeDef);
	
	
	//定时器  配置一个中断
  //打开时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);
	
	//时基配置
	TIM_TimeBaseInitTypeDef tim_TimeBaseInitTypeDef;
	tim_TimeBaseInitTypeDef.TIM_Prescaler=72-1;
	tim_TimeBaseInitTypeDef.TIM_Period=1000-1;
	tim_TimeBaseInitTypeDef.TIM_CounterMode=TIM_CounterMode_Up;
	tim_TimeBaseInitTypeDef.TIM_ClockDivision=TIM_CKD_DIV1;
	TIM_TimeBaseInit(TIM4,&tim_TimeBaseInitTypeDef);
	//CR1基础配置
	TIM_ARRPreloadConfig(TIM4,ENABLE);        //使能ARR影子寄存器
	TIM_UpdateDisableConfig(TIM4,DISABLE);    //更新禁止
	TIM_UpdateRequestConfig(TIM4,TIM_UpdateSource_Regular);  //更新请求源
	TIM_SelectOnePulseMode(TIM4,TIM_OPMode_Repetitive);      //是否重复执行计数器
	//人为产生更新事件
	TIM_GenerateEvent(TIM4,TIM_EventSource_Update);
	TIM_ClearFlag(TIM4,TIM_FLAG_Update);
	
	
	//中断配置
	TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE);  //使能更新中断
	
	NVIC_InitTypeDef nvic_InitTypeDef;
	nvic_InitTypeDef.NVIC_IRQChannel=TIM4_IRQn;
	nvic_InitTypeDef.NVIC_IRQChannelPreemptionPriority=3;
	nvic_InitTypeDef.NVIC_IRQChannelSubPriority=1;
	nvic_InitTypeDef.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&nvic_InitTypeDef);
	
	//暂时关闭计数器   控制函数中控制打开
	TIM_Cmd(TIM4,DISABLE);
	
	//每次开机回到最左边
	Stepmotor_Contrl(-800);
}



/*********************************
函数名:Step_Switch
函数功能：步进电机步进切换
形参：控制方向的参数 u8 direction
返回值：
备注：根据方向切换步进
*********************************/
void Step_Switch(u8 direction)
{
	static u8 step_counter=0;
	
	step_counter++;  
	
	if(direction == FORWARD)
	{
		//正向步进
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
		//反向步进
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
函数名:Stepmotor_Stop
函数功能：步进电机停止函数
形参：无
返回值：无
备注：停止步进电机动作
*********************************/
void Stepmotor_Stop(void)
{
	COIL_A_L; COIL_A_INV_L; COIL_B_L; COIL_B_INV_L;//0000
}



/*********************************
函数名:Stepmotor_Contrl
函数功能：步进电机控制
形参：控制方向和距离 s32 steps
返回值：无
备注：
		如果steps大于0 向前移动
    如果steps小于0 向后移动
    如果steps等于0 停止移动

考虑距离
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
函数名:TIM4_IRQHandler
函数功能：定时器2的中断服务函数
形参：
返回值：
备注：
*/
void TIM4_IRQHandler(void)
{
	static u32 interrupt_count = 0;
	//1ms进入一次
	if(TIM_GetITStatus(TIM4,TIM_IT_Update) == SET)
	{
		//清除中断挂起位
		TIM_ClearITPendingBit(TIM4,TIM_IT_Update);
		interrupt_count++;    //记录进入了多少次中断
		
		Step_Switch(motor_direction);   //根据方向执行步进切换
		
		//结束条件
		if(interrupt_count >= motor_steps)
		{
			//已经执行完毕
			interrupt_count = 0;
			Stepmotor_Stop();
      TIM_Cmd(TIM4,DISABLE);
		}
	}
}





