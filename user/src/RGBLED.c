#include "RGBLED.h"

/*********************************
��������RGBLED_Init
�������ܣ� GPIO������
�βΣ���
����ֵ����
��ע��DATA_PIN--PB8, CLK_PIN--PB9
*********************************/
void RGBLED_Init(void) 
{
	// ʹ�� GPIO ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	// ���� DATA_PIN �� CLK_PIN Ϊ�������ģʽ
	GPIO_InitStructure.GPIO_Pin   = DATA_PIN;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(DATA_PORT, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin   = CLK_PIN;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(CLK_PORT, &GPIO_InitStructure);
}

/*********************************
��������RGBLED_Show
�������ܣ� ��ʾRGB��ɫ
�βΣ�uint8_t r, uint8_t g, uint8_t b--��, ��, ��
����ֵ����
��ע��ÿ����һ�ζ�Ҫ����һ����ʼ�ź�32λ0
*********************************/
void RGBLED_Show(uint8_t r, uint8_t g, uint8_t b) 
{
	// ���ͳ�ʼ���źţ�32����ֻ�����һ��
//	static bool isInitialized = false;
//	if (!isInitialized) {
			RGBLED_SendData(0x00000000); // ����32����
//			isInitialized = true;
//	}
	
	// �ϳ� RGB ��ɫֵ
	uint32_t color = RGBLED_EncodeColor(r, g, b);
	
	// ���ͺϳɵ� RGB ��ɫֵ
	RGBLED_SendData(color);
}

/*********************************
��������RGBLED_SendData
�������ܣ� ����32λ����
�βΣ�uint32_t data
����ֵ����
��ע��32λ�����ݿ�������ʼ�ź�32λ0��
			Ҳ������32λ�ϳɵ���ɫ����
*********************************/
void RGBLED_SendData(uint32_t data)
{
	// ���� P9813 Э�鷢������
	for (int i = 31; i >= 0; i--) 
	{
		//DIN����CIN--�Ƚ�����׼����
		//��data�����λ�ж�����ʼ�źŻ�����ɫ����
		//�ȷ��͸�λ���ڷ��͵�λ
		if (data & (1 << i)) 
		{
				DATA_HIGH; // ���� 1
		} else 
		{
				DATA_LOW;  // ���� 0
		}
		CLK_HIGH;   // ������
		delay_us(10); // ��ʱ��ȷ���ź��ȶ�
		CLK_LOW;    // �½���
		delay_us(10); // ��ʱ��ȷ���ź��ȶ�
	}
}

/*********************************
��������RGBLED_EncodeColor
�������ܣ� ��r, g, b���и�ʽ����
�βΣ�u8 r, u8 g, u8 b
����ֵ��uint32_t data
��ע��32λ����
	ʱ�򣺱�־λ(11),
				��ɫ����λ�ķ���,
				��ɫ����λ�ķ���,
				��ɫ����λ�ķ��룬
				8λ��ɫ����,
				8λ��ɫ����,
				8λ��ɫ����
*********************************/
uint32_t RGBLED_EncodeColor(uint8_t r, uint8_t g, uint8_t b)
{
	// ����ÿ����ɫ��������λ�ķ���
	uint8_t r_inv = (~r & 0xC0) >> 6; // ��ɫ����λ�ķ���
	uint8_t g_inv = (~g & 0xC0) >> 6; // ��ɫ����λ�ķ���
	uint8_t b_inv = (~b & 0xC0) >> 6; // ��ɫ����λ�ķ���

	// ���� 32 λ����
	uint32_t data = 0;
	data |= (0x03 << 30); // ��־λ���� 2 λ��
	data |= (b_inv << 28); // ��ɫ���루�������� 2 λ��
	data |= (g_inv << 26); // ��ɫ���루�������� 2 λ��
	data |= (r_inv << 24); // ��ɫ���루�������� 2 λ��
	data |= (b << 16);     // ��ɫ���ݣ��������� 8 λ��
	data |= (g << 8);      // ��ɫ���ݣ��������� 8 λ��
	data |= (r);           // ��ɫ���ݣ���͵� 8 λ��

	return data;
}

/*********************************
��������NeonLightEffect
�������ܣ�ʵ���޺��Ч��
�βΣ�uint32_t delayTime ������ʱ
����ֵ����
��ע����ɫ���䣺������ɫ����ķ�Χ������Ӻ�ɫ����ɫ���ٵ���ɫ�����ص���ɫ��
			�����Ը��£������Եظ��� LED ����ɫ��ʹ����ɫ��Ԥ��ķ�Χ�ڱ仯��
			��ʱ���ƣ�ʹ����ʱ����������ɫ�仯���ٶȺ�Ч����
*********************************/
void NeonLightEffect(uint32_t delayTime)
{
 uint8_t r, g, b;
	while (1) 
	{
		for (r = 0; r < 255; r++) 
		{  // ��ɫ����
			RGBLED_Show(r, 0, 255 - r);
			delay_ms(delayTime);  // ��ʱ
		}
		for (g = 0; g < 255; g++) 
		{  // ��ɫ����
			RGBLED_Show(255 - g, g, 0);
			delay_ms(delayTime);  // ��ʱ
		}
		for (b = 0; b < 255; b++) 
		{  // ��ɫ����
			RGBLED_Show(0, 255 - b, b);
			delay_ms(delayTime);  // ��ʱ
		}
	}
}

/*********************************
��������PoliceStrobeEffect
�������ܣ�ʵ�־���������Ч��
�βΣ�uint32_t delayTime ������ʱ
����ֵ����
��ע����
*********************************/
void PoliceStrobeEffect(uint32_t delayTime) 
{
	while (1)
	{
		// ��ɫ����
		RGBLED_Show(255, 0, 0);  // ��ɫȫ��
		delay_ms(delayTime);
		RGBLED_Show(0, 0, 0);    // ȫ��
		delay_ms(delayTime);

		// ��ɫ����
		RGBLED_Show(0, 0, 255);  // ��ɫȫ��
		delay_ms(delayTime);
		RGBLED_Show(0, 0, 0);    // ȫ��
		delay_ms(delayTime);
	}
}

/*********************************
��������FireTruckWarningEffect
�������ܣ�ʵ������������Ч��
�βΣ�uint32_t delayTime ������ʱ
����ֵ����
��ע����
*********************************/
void FireTruckWarningEffect(uint32_t delayTime) 
{
	while (1) 
	{
		// ��ɫ��˸
		RGBLED_Show(255, 0, 0);  // ��ɫȫ��
		delay_ms(delayTime);
		RGBLED_Show(0, 0, 0);    // ȫ��
		delay_ms(delayTime);
	}
}