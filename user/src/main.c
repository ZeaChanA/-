#include "main.h"

int main(void)
{
	LED_Init();
	USART1_Init(9600);
	RGBLED_Init();
	DHT11_Init();
	Stepmotor_Init();
	
	//��ʪ������
	uint8_t temperature = 0; 
	uint8_t humidity = 0; 
  
	while(1)
	{
		Stepmotor_Contrl(50);
		/*
		//��������ʾ��Ч��
		FireTruckWarningEffect(100);
		
		//������Ч��
		PoliceStrobeEffect(100);

		//�޺��Ч��
		NeonLightEffect(1);
		*/
		/*
		//��ȡ��ʪ������
		DHT11_ReadData(&temperature,&humidity);
		printf("Temperature: %d��C, Humidity: %d%%\r\n", temperature, humidity); 
		delay_ms(1000); // ÿ��1���ȡһ������
		*/
	}
}
