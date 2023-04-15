#include "ht32.h"
#include "ESP8266.h"
#include "delay.h"
#include "usart.h"
#include <string.h>
#include <stdio.h>


#define ESP8266_WIFI_INFO		"AT+CWJAP=\"slow-walk\",\"yin12345\"\r\n"

#define ESP8266_ONENET_INFO		"AT+CIPSTART=\"TCP\",\"183.230.40.39\",6002\r\n"


u8 esp8266_buf[256];
u32 esp8266_cnt = 0, esp8266_cntPre = 0;

/**
 * @brief 清空缓存
 * 
 */
void ESP8266_Clear(void)
{

	memset(esp8266_buf, 0, sizeof(esp8266_buf));
	esp8266_cnt = 0;

}

/**
 * @brief 等到接收完成
 * 
 * @return u8 若返回REV_OK表示接收完成，若返回REV_WAIT表示接收超时未完成 
 */
u8 ESP8266_WaitRecive(void)
{

	if(esp8266_cnt == 0) 							//如果接收计数为0，则说明没有待接收的数据，直接结束函数
		return REV_WAIT;
		
	if(esp8266_cnt == esp8266_cntPre)				//如果上一次的值与此次相同，则说明接收完毕
	{
		esp8266_cnt = 0;							//清0接收次数
			
		return REV_OK;								//返回接收完成标志
	}
		
	esp8266_cntPre = esp8266_cnt;					//置为相同
	
	return REV_WAIT;								//返回接收未完成标志

}


u8 ESP8266_SendCmd(char *cmd, char *res)
{
	
	u8 timeOut = 200;

	Usart_SendString(COM0_PORT, (u8 *)cmd, strlen((const char *)cmd));
	
	while(timeOut--)
	{
		if(ESP8266_WaitRecive() == REV_OK)							//Èç¹ûÊÕµ½Êý¾Ý
		{
			if(strstr((const char *)esp8266_buf, res) != NULL)		//Èç¹û¼ìË÷µ½¹Ø¼ü´Ê
			{
				ESP8266_Clear();									//Çå¿Õ»º´æ
				
				return 0;
			}
		}
		
		delay_ms(10);
	}
	
	return 1;

}

//==========================================================
//	º¯ÊýÃû³Æ£º	ESP8266_SendData
//
//	º¯Êý¹¦ÄÜ£º	·¢ËÍÊý¾Ý
//
//	Èë¿Ú²ÎÊý£º	data£ºÊý¾Ý
//				len£º³¤¶È
//
//	·µ»Ø²ÎÊý£º	ÎÞ
//
//	ËµÃ÷£º		
//==========================================================
void ESP8266_SendData(u8 *data, u32 len)
{

	char cmdBuf[32];
	
	ESP8266_Clear();								//Çå¿Õ½ÓÊÕ»º´æ
	
	//ÏÈ·¢ËÍÒª·¢ËÍÊý¾ÝµÄÖ¸Áî×ö×¼±¸
	sprintf(cmdBuf, "AT+CIPSEND=%d\r\n", len);		//·¢ËÍÃüÁî
	if(!ESP8266_SendCmd(cmdBuf, ">"))				//ÊÕµ½¡®>¡¯Ê±¿ÉÒÔ·¢ËÍÊý¾Ý
	{
		//¼ÈÈ»×¼±¸Íê±Ï¼´¿É¿ªÊ¼·¢ËÍÊý¾Ý
		Usart_SendString(COM0_PORT, data, len);		//·¢ËÍÉè±¸Á¬½ÓÇëÇóÊý¾Ý
	}

}

/**
 * @brief 
 * 
 * @param timeOut 
 * @return u8* 
 */
u8 *ESP8266_GetIPD(u32 timeOut)
{

	char *ptrIPD = NULL;
	
	do
	{
		if(ESP8266_WaitRecive() == REV_OK)								//Èç¹û½ÓÊÕÍê³É
		{
			ptrIPD = strstr((char *)esp8266_buf, "IPD,");				//ËÑË÷¡°IPD¡±Í·
			if(ptrIPD == NULL)											//Èç¹ûÃ»ÕÒµ½£¬¿ÉÄÜÊÇIPDÍ·µÄÑÓ³Ù£¬»¹ÊÇÐèÒªµÈ´ýÒ»»á£¬µ«²»»á³¬¹ýÉè¶¨µÄÊ±¼ä
			{
				//UsartPrintf(USART_DEBUG, "\"IPD\" not found\r\n");
			}
			else
			{
				ptrIPD = strchr(ptrIPD, ':');							//ÕÒµ½':'
				if(ptrIPD != NULL)
				{
					ptrIPD++;
					return (u8 *)(ptrIPD);
				}
				else
					return NULL;
				
			}
		}
		
		delay_ms(5);													//ÑÓÊ±µÈ´ý
	} while(timeOut--);
	
	return NULL;														//³¬Ê±»¹Î´ÕÒµ½£¬·µ»Ø¿ÕÖ¸Õë

}

//==========================================================
//	º¯ÊýÃû³Æ£º	ESP8266_Init
//
//	º¯Êý¹¦ÄÜ£º	³õÊ¼»¯ESP8266
//
//	Èë¿Ú²ÎÊý£º	ÎÞ
//
//	·µ»Ø²ÎÊý£º	ÎÞ
//
//	ËµÃ÷£º		
//==========================================================
void ESP8266_Init(void)
{
	
	CKCU_PeripClockConfig_TypeDef CKCUClock= {{0}};
	CKCUClock.Bit.PC=1;
	CKCU_PeripClockConfig(CKCUClock, ENABLE);
	
	GPIO_DirectionConfig(HT_GPIOC, GPIO_PIN_9, GPIO_DIR_OUT); //ÉèÖÃPC15ÎªÊä³ö
	WIFI_Clr();

	delay_ms(250);
	WIFI_Set();
	delay_ms(500);
	
	ESP8266_Clear();

	UsartPrintf(USART_DEBUG, "1. AT\r\n");
	while(ESP8266_SendCmd("AT\r\n", "OK"))
		delay_ms(500);
	
	UsartPrintf(USART_DEBUG, "2. CWMODE\r\n");
	while(ESP8266_SendCmd("AT+CWMODE=1\r\n", "OK"))
		delay_ms(500);
	
	UsartPrintf(USART_DEBUG, "3. CWJAP\r\n");
	while(ESP8266_SendCmd(ESP8266_WIFI_INFO, "GOT IP"))
		delay_ms(500);
	
	UsartPrintf(USART_DEBUG, "4. CIPSTART\r\n");
	while(ESP8266_SendCmd(ESP8266_ONENET_INFO, "CONNECT"))
		delay_ms(500);
	
	UsartPrintf(USART_DEBUG, "5. ESP8266 Init OK\r\n");

}

//==========================================================
//	º¯ÊýÃû³Æ£º	USART2_IRQHandler
//
//	º¯Êý¹¦ÄÜ£º	´®¿Ú2ÊÕ·¢ÖÐ¶Ï
//
//	Èë¿Ú²ÎÊý£º	ÎÞ
//
//	·µ»Ø²ÎÊý£º	ÎÞ
//
//	ËµÃ÷£º		
//==========================================================
//void USART0_IRQHandler(void)
//{

//	if(USART_GetFlagStatus(COM0_PORT , USART_FLAG_RXDR) != RESET) //½ÓÊÕÖÐ¶Ï
//	{
//		if(gURRx_Length >= sizeof(gRx_Buffer))	gURRx_Length = 0; //·ÀÖ¹´®¿Ú±»Ë¢±¬
//		gRx_Buffer[gURRx_Length++] = COM0_PORT->DR;
//		
//		USART_ClearFlag(COM0_PORT, USART_FLAG_RSADD);
//	}

//}

void USART0_IRQHandler(void)
{

	if(USART_GetFlagStatus(COM0_PORT, USART_FLAG_RXDR) != RESET) //½ÓÊÕÖÐ¶Ï
	{
		if(esp8266_cnt >= sizeof(esp8266_buf))	esp8266_cnt = 0; //·ÀÖ¹´®¿Ú±»Ë¢±¬
		esp8266_buf[esp8266_cnt++] = COM0_PORT->DR;
		
		USART_ClearFlag(COM0_PORT, USART_FLAG_RSADD);
	}

}

