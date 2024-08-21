#include "main.h"

int main(void)
{
	LED_Init();
	USART1_Init(9600);
	RGBLED_Init();
	DHT11_Init();
	Stepmotor_Init();
	
	//温湿度数据
	uint8_t temperature = 0; 
	uint8_t humidity = 0; 
  
	while(1)
	{
		Stepmotor_Contrl(50);
		/*
		//消防车警示灯效果
		FireTruckWarningEffect(100);
		
		//爆闪灯效果
		PoliceStrobeEffect(100);

		//霓虹灯效果
		NeonLightEffect(1);
		*/
		/*
		//读取温湿度数据
		DHT11_ReadData(&temperature,&humidity);
		printf("Temperature: %d°C, Humidity: %d%%\r\n", temperature, humidity); 
		delay_ms(1000); // 每隔1秒读取一次数据
		*/
	}
}
