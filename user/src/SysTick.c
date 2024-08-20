#include "SysTick.h"

//ϵͳ��શ�ʱ�����������ӳٺ���
void delay_ms(unsigned int ms)  //��ms����us������9000����9���͸ĳ���us��ʱ����

{
	unsigned int tmp = 0;

	SysTick->LOAD = ms * 9000;    //  ��װ�Ĵ��� ���� 72MHz/9000=9MHz

	SysTick->VAL = 0x00;               // ��ǰ�����Ĵ��� ����

	SysTick->CTRL = 0x01;            // ���ƼĴ���  ʹ��systick,��ֹ�жϣ�ʱ��Ƶ��AHB/8
	do
	{
		tmp = SysTick->CTRL;         // ��ȡ���ƼĴ�����ֵ����tmp
	}
  while (!(tmp & (1<<16)));         // ���Ƕ�ȡtmp�ĵ�16λ��ֵ����һλ���Ϊ0�ͱ�ʾ��������ֵ����0(��

                                                 // ���ڼ���)�������1�ͱ�ʾ�������Ѿ��Լ���0��,����������

                                                  //�����ֵΪ0�����ڼ�������������ģ�ȡ��Ϊ1��������do�����ݡ�

  SysTick->VAL = 0x00;             // ��ǰ�����Ĵ��� ����
  SysTick->CTRL = 0x00;          //ʧ��systick
}


//ϵͳ��શ�ʱ������΢���ӳٺ���
void delay_us(unsigned int us)  //��ms����us������9000����9���͸ĳ���us��ʱ����

{
	unsigned int tmp = 0;

	SysTick->LOAD = us * 9;    //  ��װ�Ĵ��� ���� 72MHz/9000=9MHz

	SysTick->VAL = 0x00;               // ��ǰ�����Ĵ��� ����

	SysTick->CTRL = 0x01;            // ���ƼĴ���  ʹ��systick,��ֹ�жϣ�ʱ��Ƶ��AHB/8
	do
	{
		tmp = SysTick->CTRL;         // ��ȡ���ƼĴ�����ֵ����tmp
	}
  while (!(tmp & (1<<16)));         // ���Ƕ�ȡtmp�ĵ�16λ��ֵ����һλ���Ϊ0�ͱ�ʾ��������ֵ����0(��

                                                 // ���ڼ���)�������1�ͱ�ʾ�������Ѿ��Լ���0��,����������

                                                  //�����ֵΪ0�����ڼ�������������ģ�ȡ��Ϊ1��������do�����ݡ�

  SysTick->VAL = 0x00;             // ��ǰ�����Ĵ��� ����
  SysTick->CTRL = 0x00;          //ʧ��systick
}



//// SysTick �жϴ������
//void SysTick_Handler(void) {
//    uwTick++;
//}