#include "usart.h"
#include "ht32f5xxxx_gpio.h"
#include <stdarg.h>

/**************************实现函数********************************************
函数说明：配置usart串口

*******************************************************************************/ 
void USART_Configuration(void)
{
	 USART_InitTypeDef USART_InitStructure;
    {                                                              
        CKCU_PeripClockConfig_TypeDef CKCUClock = {{0}};
        CKCUClock.Bit.AFIO = 1;
        CKCUClock.Bit.PA = 1;
        CKCUClock.Bit.PC = 1;
        CKCUClock.Bit.USART1 = 1;
        CKCUClock.Bit.USART0 = 1;
        CKCU_PeripClockConfig(CKCUClock, ENABLE);
    }
	 
    GPIO_PullResistorConfig(HT_GPIOC, USART0_RX_PIN, GPIO_PR_UP);
    GPIO_PullResistorConfig(HT_GPIOA, USART1_RX_PIN, GPIO_PR_UP);

    AFIO_GPxConfig(USART0_GPIO_GROUP, USART0_TX_PIN, USART_AFIO_MODE);
    AFIO_GPxConfig(USART0_GPIO_GROUP, USART0_RX_PIN, USART_AFIO_MODE);
    AFIO_GPxConfig(USART1_GPIO_GROUP, USART1_TX_PIN, USART_AFIO_MODE);
    AFIO_GPxConfig(USART1_GPIO_GROUP, USART1_RX_PIN, USART_AFIO_MODE);

  /*
		bound：     115200
		数据长度：   8bits
        停止位：    1位
        校验位：    无
        模式：      正常模式
  */
		
    USART_InitStructure.USART_BaudRate = 115200;
    USART_InitStructure.USART_WordLength = USART_WORDLENGTH_8B;
    USART_InitStructure.USART_StopBits = USART_STOPBITS_1;
    USART_InitStructure.USART_Parity = USART_PARITY_NO;
    USART_InitStructure.USART_Mode = USART_MODE_NORMAL;
    USART_Init(HT_USART1, &USART_InitStructure);
    USART_Init(HT_USART0, &USART_InitStructure);
  
        //中断设置
		NVIC_EnableIRQ(USART1_IRQn);
		NVIC_EnableIRQ(USART0_IRQn);
			
		USART_IntConfig(HT_USART1, USART_INT_RXDR, ENABLE);
		USART_IntConfig(HT_USART0, USART_INT_RXDR, ENABLE);
			
		//使能 串口 发送和接收
		USART_TxCmd(HT_USART1, ENABLE);
		USART_RxCmd(HT_USART1, ENABLE);
		USART_TxCmd(HT_USART0, ENABLE);
		USART_RxCmd(HT_USART0, ENABLE);
	

	 	                                                                                      
//  USART_RXTLConfig(COM1_PORT, USART_RXTL_04); //设置FIFO接收等级

}



//串口0用作ESP8266通讯接口，中断服务函数改设在ESP8266.c
// void USART0_IRQHandler(void)			//串口0中断服务函数
// {
// 	u8 data;
	
// 	if( USART_GetFlagStatus(HT_USART0, USART_FLAG_RXDR ) )         //接收中断
// 	{
// 		data = USART_ReceiveData(HT_USART0);                         //读取接收到的数据
// 		Usart_Sendbyte(HT_USART0,data);                                 //把收到的数据发送回上位机		
//     }
// 	USART_ClearFlag(HT_USART0,USART_FLAG_RXDR);
// }

void USART_Tx1(const char* TxBuffer, u32 length)
{
  u32 i;
  for (i = 0; i < length; i++)
  {
    while (!USART_GetFlagStatus(HT_USART1, USART_FLAG_TXC));
    USART_SendData(HT_USART1, TxBuffer[i]);
  }
}

void USART_Tx0(const char* TxBuffer, u32 length)
{
    u32 i;
    for (i = 0; i < length; i++)
    {
        while (!USART_GetFlagStatus(HT_USART0, USART_FLAG_TXC));
        USART_SendData(HT_USART0, TxBuffer[i]);
    }
}

/**************************实现函数********************************************
函数说明：发送一个字节

*******************************************************************************/ 
void Usart_Sendbyte(HT_USART_TypeDef* USARTx, u8 data)
{
    USART_SendData(USARTx, data);
    while (USART_GetFlagStatus(USARTx, USART_FLAG_TXDE) == RESET);		
}
 
/**************************实现函数********************************************
函数说明：发送数组

*******************************************************************************/ 
void Usart_SendArray(HT_USART_TypeDef* USARTx, u8 *array, u8 num)
{
	u8 i;
	for(i = 0; i < num; i++)
	{
		Usart_Sendbyte(USARTx, *array);
		array++;
	}
}

 /**************************实现函数********************************************
函数说明：发送字符串

*******************************************************************************/ 
void Usart_SendStr(HT_USART_TypeDef* USARTx, uint8_t *str)
{
	uint8_t i;
	for(i = 0; str[i] != '\0'; i++)
	{
		Usart_Sendbyte(USARTx, str[i]);
	}
}

void Usart_SendString(HT_USART_TypeDef *USARTx, unsigned char *str, unsigned short len)
{

	unsigned short count = 0;
	
	for(; count < len; count++)
	{
		USART_SendData(USARTx, *str++);									//发送数据
		while(USART_GetFlagStatus(USARTx, USART_FLAG_TXDE) == RESET);		//等待发送完成
	}

}


/*
************************************************************
*	函数名称：	UsartPrintf
*
*	函数功能：	格式化打印信息，便于调试
*
*	入口参数：	USARTx：串口组
*				fmt：不定长参
*
*	返回参数：	无
*
*	说明：		
************************************************************
*/
void UsartPrintf(HT_USART_TypeDef* USARTx, char *fmt,...)
{

	unsigned char UsartPrintfBuf[296];
	va_list ap;
	unsigned char *pStr = UsartPrintfBuf;
	
	va_start(ap, fmt);
	vsnprintf((char *)UsartPrintfBuf, sizeof(UsartPrintfBuf), fmt, ap);							//格式化
	va_end(ap);
	
	while(*pStr != 0)
	{
		USART_SendData(USARTx, *pStr++);
		while(USART_GetFlagStatus(USARTx, USART_FLAG_TXDE) == RESET);
	}

}

/**************************实现函数********************************************
函数说明：接收中断服务函数

*******************************************************************************/ 
//void USART1_IRQHandler(void)			//串口1中断服务函数
//{
//	u8 data;
//	
//	if( USART_GetFlagStatus(HT_USART1, USART_FLAG_RXDR ) )         //接收中断
//	{
//		data = USART_ReceiveData(HT_USART1);                         //读取接收到的数据
////		Usart_Sendbyte(HT_USART0,data);                                 //把收到的数据发送回上位机		
//	}
//	USART_ClearFlag(HT_USART1,USART_FLAG_RXDR);
//}
