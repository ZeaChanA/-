#include "RGBLED.h"

/*********************************
函数名：RGBLED_Init
函数功能： GPIO口配置
形参：无
返回值：无
备注：DATA_PIN--PB8, CLK_PIN--PB9
*********************************/
void RGBLED_Init(void) 
{
	// 使能 GPIO 时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	// 配置 DATA_PIN 和 CLK_PIN 为推挽输出模式
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
函数名：RGBLED_Show
函数功能： 显示RGB颜色
形参：uint8_t r, uint8_t g, uint8_t b--红, 绿, 蓝
返回值：无
备注：每调用一次都要发送一次起始信号32位0
*********************************/
void RGBLED_Show(uint8_t r, uint8_t g, uint8_t b) 
{
	// 发送初始化信号，32个零只需调用一次
//	static bool isInitialized = false;
//	if (!isInitialized) {
			RGBLED_SendData(0x00000000); // 发送32个零
//			isInitialized = true;
//	}
	
	// 合成 RGB 颜色值
	uint32_t color = RGBLED_EncodeColor(r, g, b);
	
	// 发送合成的 RGB 颜色值
	RGBLED_SendData(color);
}

/*********************************
函数名：RGBLED_SendData
函数功能： 发送32位数据
形参：uint32_t data
返回值：无
备注：32位的数据可以是起始信号32位0，
			也可以是32位合成的颜色数据
*********************************/
void RGBLED_SendData(uint32_t data)
{
	// 按照 P9813 协议发送数据
	for (int i = 31; i >= 0; i--) 
	{
		//DIN先于CIN--先将数据准备好
		//用data的最高位判断是起始信号还是颜色数据
		//先发送高位，在发送地位
		if (data & (1 << i)) 
		{
				DATA_HIGH; // 发送 1
		} else 
		{
				DATA_LOW;  // 发送 0
		}
		CLK_HIGH;   // 上升沿
		delay_us(10); // 延时以确保信号稳定
		CLK_LOW;    // 下降沿
		delay_us(10); // 延时以确保信号稳定
	}
}

/*********************************
函数名：RGBLED_EncodeColor
函数功能： 对r, g, b进行格式编码
形参：u8 r, u8 g, u8 b
返回值：uint32_t data
备注：32位编码
	时序：标志位(11),
				蓝色高两位的反码,
				绿色高两位的反码,
				蓝色高两位的反码，
				8位蓝色数据,
				8位绿色数据,
				8位红色数据
*********************************/
uint32_t RGBLED_EncodeColor(uint8_t r, uint8_t g, uint8_t b)
{
	// 计算每个颜色分量高两位的反码
	uint8_t r_inv = (~r & 0xC0) >> 6; // 红色高两位的反码
	uint8_t g_inv = (~g & 0xC0) >> 6; // 绿色高两位的反码
	uint8_t b_inv = (~b & 0xC0) >> 6; // 蓝色高两位的反码

	// 构造 32 位数据
	uint32_t data = 0;
	data |= (0x03 << 30); // 标志位（高 2 位）
	data |= (b_inv << 28); // 蓝色反码（接下来的 2 位）
	data |= (g_inv << 26); // 绿色反码（接下来的 2 位）
	data |= (r_inv << 24); // 红色反码（接下来的 2 位）
	data |= (b << 16);     // 蓝色数据（接下来的 8 位）
	data |= (g << 8);      // 绿色数据（接下来的 8 位）
	data |= (r);           // 红色数据（最低的 8 位）

	return data;
}

/*********************************
函数名：NeonLightEffect
函数功能：实现霓虹灯效果
形参：uint32_t delayTime 毫秒延时
返回值：无
备注：颜色渐变：定义颜色渐变的范围，比如从红色到绿色，再到蓝色，最后回到红色。
			周期性更新：周期性地更新 LED 的颜色，使其颜色在预设的范围内变化。
			延时控制：使用延时函数控制颜色变化的速度和效果。
*********************************/
void NeonLightEffect(uint32_t delayTime)
{
 uint8_t r, g, b;
	while (1) 
	{
		for (r = 0; r < 255; r++) 
		{  // 红色渐变
			RGBLED_Show(r, 0, 255 - r);
			delay_ms(delayTime);  // 延时
		}
		for (g = 0; g < 255; g++) 
		{  // 绿色渐变
			RGBLED_Show(255 - g, g, 0);
			delay_ms(delayTime);  // 延时
		}
		for (b = 0; b < 255; b++) 
		{  // 蓝色渐变
			RGBLED_Show(0, 255 - b, b);
			delay_ms(delayTime);  // 延时
		}
	}
}

/*********************************
函数名：PoliceStrobeEffect
函数功能：实现警车爆闪灯效果
形参：uint32_t delayTime 毫秒延时
返回值：无
备注：无
*********************************/
void PoliceStrobeEffect(uint32_t delayTime) 
{
	while (1)
	{
		// 红色爆闪
		RGBLED_Show(255, 0, 0);  // 红色全亮
		delay_ms(delayTime);
		RGBLED_Show(0, 0, 0);    // 全灭
		delay_ms(delayTime);

		// 蓝色爆闪
		RGBLED_Show(0, 0, 255);  // 蓝色全亮
		delay_ms(delayTime);
		RGBLED_Show(0, 0, 0);    // 全灭
		delay_ms(delayTime);
	}
}

/*********************************
函数名：FireTruckWarningEffect
函数功能：实现消防车警灯效果
形参：uint32_t delayTime 毫秒延时
返回值：无
备注：无
*********************************/
void FireTruckWarningEffect(uint32_t delayTime) 
{
	while (1) 
	{
		// 红色闪烁
		RGBLED_Show(255, 0, 0);  // 红色全亮
		delay_ms(delayTime);
		RGBLED_Show(0, 0, 0);    // 全灭
		delay_ms(delayTime);
	}
}