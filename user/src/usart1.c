#include "usart1.h"

/*********************************
函数名：USART1_Init
函数功能：USART1初始化
形参：uint32_t baudRate --波特率
返回值：无
备注：PA9--USART1-TX
			PA10--USART1-RX
*********************************/
void USART1_Init(uint32_t baudRate)
{
	//使能外设时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);
	
	//配置GPIO
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_9;
	GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_AF_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_10;
	GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	
	//配置USART1
	USART_InitTypeDef USART_InitStruct;
	USART_InitStruct.USART_BaudRate = baudRate;
	USART_InitStruct.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
	USART_InitStruct.USART_Parity = USART_Parity_No;
	USART_InitStruct.USART_StopBits = USART_StopBits_1;
	USART_InitStruct.USART_WordLength = USART_WordLength_8b;
	USART_Init(USART1, &USART_InitStruct);
	
	//使能USART1
	USART_Cmd(USART1, ENABLE);
}

/*********************************
函数名：USART_SendByte
函数功能：USART发送字节
形参：USART_TypeDef *USARTx，uint8_t data
返回值：无
备注：USART1,2,3通用
*********************************/

void USART_SendByte(USART_TypeDef *USARTx, uint8_t data)
{
	//等待发送寄存器为空
	while (USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);
	
	//发送数据
	USART_SendData(USARTx, data);
	
}

/*********************************
函数名：USART_SendString
函数功能：USART发送字符串
形参：USART_TypeDef *USARTx，const char *str
返回值：无
备注：USART1,2,3通用
*********************************/
void USART_SendString(USART_TypeDef *USARTx, const char *str)
{
	while (*str)
	{
		 USART_SendByte(USARTx, (uint8_t)(*str++));
	}
}

/*********************************
函数名：USART_ReceiveByte
函数功能：USART接收字节
形参：USART_TypeDef *USARTx
返回值：uint8_t
备注：USART1,2,3通用
*********************************/
uint8_t USART_ReceiveByte(USART_TypeDef *USARTx)
{
	//等待接收寄存器为空
	while (USART_GetFlagStatus(USARTx, USART_FLAG_RXNE) == RESET);
	
	//读取数据
	return (uint8_t)USART_ReceiveData(USARTx);
	
}

/*********************************
函数名：USART_ReceiveString
函数功能：USART接收字符串
形参：USART_TypeDef *USARTx, char *buffer, uint16_t maxLength
返回值：无
备注：USART1,2,3通用
*********************************/
void USART_ReceiveString(USART_TypeDef *USARTx, char *buffer, uint16_t maxLength)
{
	uint16_t index = 0;
    while (index < (maxLength - 1)) {
        char ch = USART_ReceiveByte(USARTx);
        if (ch == '\n') { // 行结束符
            break;
        }
        buffer[index++] = ch;
    }
    buffer[index] = '\0'; // 添加字符串结束符
}

/*********************************
函数名：fputc
函数功能：重定向 printf 到 USART1
形参：nt ch, FILE *f
返回值：int
备注：
*********************************/

int fputc(int ch, FILE *f)
{
    // 将字符发送到 USART1
    USART_SendData(USART1, (uint8_t) ch);
    
    // 等待数据发送完毕
    while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);

    return ch;
}