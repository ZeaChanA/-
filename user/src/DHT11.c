#include "DHT11.h"

/*********************************
函数名：DHT11_Init
函数功能：DHT11引脚初始化
形参：无
返回值：无
备注：PB3
*********************************/
void DHT11_Init(void)
{
	//使能时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);
	//打开SW-DP，关闭JTAG-DP
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);
	//GPIO配置
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_OD;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStruct);
	DHT11_H;
	delay_ms(1000);
}

/*********************************
函数名：GPIO_SetOutput
函数功能： 配置GPIO为输出模式
形参：无
返回值：无
备注：PB3
*********************************/
void GPIO_SetOutput(void)
{
	//GPIO配置
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_OD;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStruct);
}

/*********************************
函数名：GPIO_SetIntput
函数功能： 配置GPIO为输入模式
形参：无
返回值：无
备注：PB3
*********************************/
void GPIO_SetInput(void)
{
	//GPIO配置
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStruct);
}

/*********************************
函数名：DHT11_Start
函数功能： 主机发送启动信号
形参：无
返回值：无
备注：PB3
*********************************/
void DHT11_Start(void)
{
	// 设置PA1为输出模式
	GPIO_SetOutput();
	// 拉低PA1
	DHT11_L;
	// 延时18ms
	delay_ms(18);
	// 拉高PA1
	DHT11_H;
	//延时20μs
	delay_us(20);
	// 设置PA1为输入模式
	GPIO_SetInput();
}

/*********************************
函数名：DHT11_CheckResponse
函数功能： 检测DHT发出的响应信号
形参：无
返回值：uint8_t
备注：PB3
*********************************/
uint8_t DHT11_CheckResponse(void) 
{
	uint8_t response = 0; 
	// 等待40μs
	delay_us(40); 
	// 检查PA1是否为低电平
	if (!GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_3)) 
	{
		
		// 等待80μs	
		delay_us(80);
		// 检查PA1是否为高电平
		if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_3)) 
		{
			 // DHT11响应信号正确
			 response = 1;
		} 
	}
		// 等待PA1变为低电平
		while (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_3)); 
		return response; 
}

/*********************************
函数名：DHT11_ReadByte
函数功能： 读取单个字节的DHT11数据
形参：无
返回值：uint8_t
备注：PB3
*********************************/
uint8_t DHT11_ReadByte(void) 
{
	uint8_t i, byte = 0;
	for (i=0; i<8; i++)
	{
		//响应成功后，DHT11 将数据线拉低 50 微秒
		//等待 PA1 变高
		while (!GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_3));
		// 延时 40 微秒
		//如果高电平持续时间较短（26-28 微秒），表示数据位         为 0；如果持续时间较长（70 微秒），表示数据位为 1。
		delay_us(40);
		//读取数据位
		if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_3)) 
		{
			// 如果为高电平，设置相应的位
			byte |= (1 << (7 - i));
		}
		//等待PA1变低
		while (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_3));
	}
	return byte;
}

/*********************************
函数名：DHT11_ReadByte
函数功能： 读取5个字节的DHT11数据
形参：uint8_t *temperature, uint8_t  *humidity
返回值：无
备注：PB3
*********************************/
void DHT11_ReadData(uint8_t *temperature, uint8_t *humidity) 
{
	//声明一个长度为 5 的数组 `data` 用于存储从 DHT11 读取的 5 个字节的数据。初始化为 0。
	uint8_t data[5] = {0};
	//发送启动信号给 DHT11 传感器
	DHT11_Start();
	//检查 DHT11 是否正确响应启动信号
	if (DHT11_CheckResponse())
	{
		for (int i=0; i<5; i++)
		{
			//循环 5 次，每次调用 `DHT11_ReadByte()`函数读取一个字节的数据，并存储到 `data` 数组中。
			//DHT11 传感器会依次发送湿度整数部分、湿度小数部分、温度整数部分、温度小数部分、校验和。
			data[i] = DHT11_ReadByte();
		}
		//校验数据的有效性。DHT11 传感器的最后一个字节是前四个字节的校验和。如果校验和正确，说明数据有效。
		if (( data[0] + data[1] + data[2] + data[3]) == data[4])
		{
			//如果校验和正确，将读取到的湿度整数部分赋值给传入的 `humidity` 指针，
			//将温度整数部分赋值给传入的 `temperature` 指针。不要小数部分
			*humidity = data[0];
			*temperature = data[2];
		}
	}
}