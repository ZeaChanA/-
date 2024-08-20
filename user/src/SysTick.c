#include "SysTick.h"

//系统嘀嗒定时器——毫秒延迟函数
void delay_ms(unsigned int ms)  //把ms换成us，下面9000换成9，就改成了us定时函数

{
	unsigned int tmp = 0;

	SysTick->LOAD = ms * 9000;    //  重装寄存器 设置 72MHz/9000=9MHz

	SysTick->VAL = 0x00;               // 当前计数寄存器 清零

	SysTick->CTRL = 0x01;            // 控制寄存器  使能systick,禁止中断，时钟频率AHB/8
	do
	{
		tmp = SysTick->CTRL;         // 读取控制寄存器的值赋给tmp
	}
  while (!(tmp & (1<<16)));         // 就是读取tmp的第16位的值，这一位如果为0就表示计数器的值不是0(即

                                                 // 还在计数)，如果是1就表示计数器已经自减到0了,计数结束。

                                                  //假设该值为0，还在计数，套上外面的！取反为1，继续做do的内容。

  SysTick->VAL = 0x00;             // 当前计数寄存器 清零
  SysTick->CTRL = 0x00;          //失能systick
}


//系统嘀嗒定时器——微妙延迟函数
void delay_us(unsigned int us)  //把ms换成us，下面9000换成9，就改成了us定时函数

{
	unsigned int tmp = 0;

	SysTick->LOAD = us * 9;    //  重装寄存器 设置 72MHz/9000=9MHz

	SysTick->VAL = 0x00;               // 当前计数寄存器 清零

	SysTick->CTRL = 0x01;            // 控制寄存器  使能systick,禁止中断，时钟频率AHB/8
	do
	{
		tmp = SysTick->CTRL;         // 读取控制寄存器的值赋给tmp
	}
  while (!(tmp & (1<<16)));         // 就是读取tmp的第16位的值，这一位如果为0就表示计数器的值不是0(即

                                                 // 还在计数)，如果是1就表示计数器已经自减到0了,计数结束。

                                                  //假设该值为0，还在计数，套上外面的！取反为1，继续做do的内容。

  SysTick->VAL = 0x00;             // 当前计数寄存器 清零
  SysTick->CTRL = 0x00;          //失能systick
}



//// SysTick 中断处理程序
//void SysTick_Handler(void) {
//    uwTick++;
//}