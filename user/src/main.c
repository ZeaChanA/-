#include "main.h"

int main(void)
{
	LED_Init();
	USART1_Init(9600);
	DHT11_Init();
	
	uint8_t temperature = 0; 
	uint8_t humidity = 0; 
  
	while(1)
	{
		DHT11_ReadData(&temperature,&humidity);
		printf("Temperature: %d°C, Humidity: %d%%\r\n", temperature, humidity); 
		delay_ms(1000); // 每隔1秒读取一次数据
	}
}
