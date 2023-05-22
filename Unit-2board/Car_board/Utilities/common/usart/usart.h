/*
 * @Author: lantiansb lantiansb@foxmail.com
 * @Date: 2023-03-15 15:41:11
 * @LastEditors: lantiansb lantiansb@foxmail.com
 * @LastEditTime: 2023-04-01 00:30:02
 */
#ifndef __USART_H
#define __USART_H

#include "ht32f5xxxx_usart.h"
#include "sys.h"

//WiFi模块使用
#define USART0_GPIO_GROUP             (GPIO_PB)
#define USART0_TX_PIN                 (GPIO_PIN_0)
#define USART0_RX_PIN                 (GPIO_PIN_1)

//用于双板通信
#define USART1_GPIO_GROUP             (GPIO_PA)
#define USART1_TX_PIN                 (GPIO_PIN_4)
#define USART1_RX_PIN                 (GPIO_PIN_5)

#define USART_AFIO_MODE              (AFIO_FUN_USART_UART) //配置IO工作模式，选择usart

void USART_Configuration(void);
void Usart_Sendbyte(HT_USART_TypeDef* USARTx, u8 data);
void Usart_SendArray(HT_USART_TypeDef* USARTx, u8 *array,u8 num);
void Usart_SendStr(HT_USART_TypeDef* USARTx, uint8_t *str);
void USART_Tx1(const char* TxBuffer, u32 length);
void USART_Tx0(const char* TxBuffer, u32 length);
void Usart_SendString(HT_USART_TypeDef *USARTx, unsigned char *str, unsigned short len);
void UsartPrintf(HT_USART_TypeDef* USARTx, char *fmt,...);

#endif
