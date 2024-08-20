#include "usart1.h"

/*********************************
��������USART1_Init
�������ܣ�USART1��ʼ��
�βΣ�uint32_t baudRate --������
����ֵ����
��ע��PA9--USART1-TX
			PA10--USART1-RX
*********************************/
void USART1_Init(uint32_t baudRate)
{
	//ʹ������ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);
	
	//����GPIO
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_9;
	GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_AF_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_10;
	GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	
	//����USART1
	USART_InitTypeDef USART_InitStruct;
	USART_InitStruct.USART_BaudRate = baudRate;
	USART_InitStruct.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
	USART_InitStruct.USART_Parity = USART_Parity_No;
	USART_InitStruct.USART_StopBits = USART_StopBits_1;
	USART_InitStruct.USART_WordLength = USART_WordLength_8b;
	USART_Init(USART1, &USART_InitStruct);
	
	//ʹ��USART1
	USART_Cmd(USART1, ENABLE);
}

/*********************************
��������USART_SendByte
�������ܣ�USART�����ֽ�
�βΣ�USART_TypeDef *USARTx��uint8_t data
����ֵ����
��ע��USART1,2,3ͨ��
*********************************/

void USART_SendByte(USART_TypeDef *USARTx, uint8_t data)
{
	//�ȴ����ͼĴ���Ϊ��
	while (USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);
	
	//��������
	USART_SendData(USARTx, data);
	
}

/*********************************
��������USART_SendString
�������ܣ�USART�����ַ���
�βΣ�USART_TypeDef *USARTx��const char *str
����ֵ����
��ע��USART1,2,3ͨ��
*********************************/
void USART_SendString(USART_TypeDef *USARTx, const char *str)
{
	while (*str)
	{
		 USART_SendByte(USARTx, (uint8_t)(*str++));
	}
}

/*********************************
��������USART_ReceiveByte
�������ܣ�USART�����ֽ�
�βΣ�USART_TypeDef *USARTx
����ֵ��uint8_t
��ע��USART1,2,3ͨ��
*********************************/
uint8_t USART_ReceiveByte(USART_TypeDef *USARTx)
{
	//�ȴ����ռĴ���Ϊ��
	while (USART_GetFlagStatus(USARTx, USART_FLAG_RXNE) == RESET);
	
	//��ȡ����
	return (uint8_t)USART_ReceiveData(USARTx);
	
}

/*********************************
��������USART_ReceiveString
�������ܣ�USART�����ַ���
�βΣ�USART_TypeDef *USARTx, char *buffer, uint16_t maxLength
����ֵ����
��ע��USART1,2,3ͨ��
*********************************/
void USART_ReceiveString(USART_TypeDef *USARTx, char *buffer, uint16_t maxLength)
{
	uint16_t index = 0;
    while (index < (maxLength - 1)) {
        char ch = USART_ReceiveByte(USARTx);
        if (ch == '\n') { // �н�����
            break;
        }
        buffer[index++] = ch;
    }
    buffer[index] = '\0'; // ����ַ���������
}

/*********************************
��������fputc
�������ܣ��ض��� printf �� USART1
�βΣ�nt ch, FILE *f
����ֵ��int
��ע��
*********************************/

int fputc(int ch, FILE *f)
{
    // ���ַ����͵� USART1
    USART_SendData(USART1, (uint8_t) ch);
    
    // �ȴ����ݷ������
    while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);

    return ch;
}