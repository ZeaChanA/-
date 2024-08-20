#include "main.h"

int main(void)
{
	LED_Init();
	USART1_Init(9600);
  
	while(1)
	{
		delay_ms(1000);
		printf("2\r\n");
	}
}
