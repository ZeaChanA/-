#include "DHT11.h"

/*********************************
��������DHT11_Init
�������ܣ�DHT11���ų�ʼ��
�βΣ���
����ֵ����
��ע��PB3
*********************************/
void DHT11_Init(void)
{
	//ʹ��ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);
	//��SW-DP���ر�JTAG-DP
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);
	//GPIO����
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_OD;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStruct);
	DHT11_H;
	delay_ms(1000);
}

/*********************************
��������GPIO_SetOutput
�������ܣ� ����GPIOΪ���ģʽ
�βΣ���
����ֵ����
��ע��PB3
*********************************/
void GPIO_SetOutput(void)
{
	//GPIO����
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_OD;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStruct);
}

/*********************************
��������GPIO_SetIntput
�������ܣ� ����GPIOΪ����ģʽ
�βΣ���
����ֵ����
��ע��PB3
*********************************/
void GPIO_SetInput(void)
{
	//GPIO����
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStruct);
}

/*********************************
��������DHT11_Start
�������ܣ� �������������ź�
�βΣ���
����ֵ����
��ע��PB3
*********************************/
void DHT11_Start(void)
{
	// ����PA1Ϊ���ģʽ
	GPIO_SetOutput();
	// ����PA1
	DHT11_L;
	// ��ʱ18ms
	delay_ms(18);
	// ����PA1
	DHT11_H;
	//��ʱ20��s
	delay_us(20);
	// ����PA1Ϊ����ģʽ
	GPIO_SetInput();
}

/*********************************
��������DHT11_CheckResponse
�������ܣ� ���DHT��������Ӧ�ź�
�βΣ���
����ֵ��uint8_t
��ע��PB3
*********************************/
uint8_t DHT11_CheckResponse(void) 
{
	uint8_t response = 0; 
	// �ȴ�40��s
	delay_us(40); 
	// ���PA1�Ƿ�Ϊ�͵�ƽ
	if (!GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_3)) 
	{
		
		// �ȴ�80��s	
		delay_us(80);
		// ���PA1�Ƿ�Ϊ�ߵ�ƽ
		if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_3)) 
		{
			 // DHT11��Ӧ�ź���ȷ
			 response = 1;
		} 
	}
		// �ȴ�PA1��Ϊ�͵�ƽ
		while (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_3)); 
		return response; 
}

/*********************************
��������DHT11_ReadByte
�������ܣ� ��ȡ�����ֽڵ�DHT11����
�βΣ���
����ֵ��uint8_t
��ע��PB3
*********************************/
uint8_t DHT11_ReadByte(void) 
{
	uint8_t i, byte = 0;
	for (i=0; i<8; i++)
	{
		//��Ӧ�ɹ���DHT11 ������������ 50 ΢��
		//�ȴ� PA1 ���
		while (!GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_3));
		// ��ʱ 40 ΢��
		//����ߵ�ƽ����ʱ��϶̣�26-28 ΢�룩����ʾ����λ         Ϊ 0���������ʱ��ϳ���70 ΢�룩����ʾ����λΪ 1��
		delay_us(40);
		//��ȡ����λ
		if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_3)) 
		{
			// ���Ϊ�ߵ�ƽ��������Ӧ��λ
			byte |= (1 << (7 - i));
		}
		//�ȴ�PA1���
		while (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_3));
	}
	return byte;
}

/*********************************
��������DHT11_ReadByte
�������ܣ� ��ȡ5���ֽڵ�DHT11����
�βΣ�uint8_t *temperature, uint8_t  *humidity
����ֵ����
��ע��PB3
*********************************/
void DHT11_ReadData(uint8_t *temperature, uint8_t *humidity) 
{
	//����һ������Ϊ 5 ������ `data` ���ڴ洢�� DHT11 ��ȡ�� 5 ���ֽڵ����ݡ���ʼ��Ϊ 0��
	uint8_t data[5] = {0};
	//���������źŸ� DHT11 ������
	DHT11_Start();
	//��� DHT11 �Ƿ���ȷ��Ӧ�����ź�
	if (DHT11_CheckResponse())
	{
		for (int i=0; i<5; i++)
		{
			//ѭ�� 5 �Σ�ÿ�ε��� `DHT11_ReadByte()`������ȡһ���ֽڵ����ݣ����洢�� `data` �����С�
			//DHT11 �����������η���ʪ���������֡�ʪ��С�����֡��¶��������֡��¶�С�����֡�У��͡�
			data[i] = DHT11_ReadByte();
		}
		//У�����ݵ���Ч�ԡ�DHT11 �����������һ���ֽ���ǰ�ĸ��ֽڵ�У��͡����У�����ȷ��˵��������Ч��
		if (( data[0] + data[1] + data[2] + data[3]) == data[4])
		{
			//���У�����ȷ������ȡ����ʪ���������ָ�ֵ������� `humidity` ָ�룬
			//���¶��������ָ�ֵ������� `temperature` ָ�롣��ҪС������
			*humidity = data[0];
			*temperature = data[2];
		}
	}
}