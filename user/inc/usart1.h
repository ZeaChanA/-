#ifndef USART1_H
#define USART1_H

#include "stm32f10x.h"
#include <stdio.h>

void USART1_Init(uint32_t baudRate);
void USART_SendByte(USART_TypeDef *USARTx, uint8_t data);
void USART_SendString(USART_TypeDef *USARTx, const char *str);
uint8_t USART_ReceiveByte(USART_TypeDef *USARTx);
void USART_ReceiveString(USART_TypeDef *USARTx, char *buffer, uint16_t maxLength);
int fputc(int ch, FILE *f);

#endif