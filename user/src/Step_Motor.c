#include "Step_Motor.h"


/*
函数名:Stepmotor_Init
函数功能：步进电机的初始化
形参：
返回值：
备注：
A   PA7
A_  PA6
B   PA5
B_  PA4
*/
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



/*
函数名:Beat_Ch
函数功能：节拍切换函数
形参：控制方向的参数 u8 dir
返回值：
备注：
*/
void Beat_Ch(u8 dir)
{
	static u8 cnt=0;
	
	cnt++;  
	
	if(dir == TURN)
	{
		//调用1~4节拍切换
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
		//调用4~1的节拍切换
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
函数名:Stemp_Stop
函数功能：步进电机停止函数
形参：void
返回值：
备注：
*/
void Stemp_Stop(void)
{
	A_L;_A_L;B_L;_B_L;
}



/*
函数名:Stepmotor_Contrl
函数功能：步进电机控制函数
形参：控制方向加距离  正负控制方向  大小控制距离
s32 light
返回值：
备注：
如果大于0  向左移动
如果小于0  向右移动
如果等于0  不移动

light---以节拍为单位

考虑距离
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
函数名:TIM4_IRQHandler
函数功能：定时器2的中断服务函数
形参：
返回值：
备注：
*/
void TIM4_IRQHandler(void)
{
	static u32 n=0;
	//1ms进入一次
	if(TIM_GetITStatus(TIM4,TIM_IT_Update)==SET)
	{
		//清除中断挂起位
		TIM_ClearITPendingBit(TIM4,TIM_IT_Update);
		n++;    //记录进入了多少次中断
		
		Beat_Ch(dir);   //节拍切换
		
		//结束条件
		if(n>=dis)
		{
			//已经执行完毕
			n=0;
			Stemp_Stop();
      TIM_Cmd(TIM4,DISABLE);
		}
	}
}





