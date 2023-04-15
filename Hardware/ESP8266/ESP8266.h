#ifndef _ESP8266_H_
#define _ESP8266_H_

//ESP8266通讯接口开关
#define WIFI_Clr() GPIO_ClearOutBits(HT_GPIOC,GPIO_PIN_9)
#define WIFI_Set() GPIO_SetOutBits(HT_GPIOC,GPIO_PIN_9)


#define REV_OK		0	//接收完成标志
#define REV_WAIT	1	//接收未完成标志

#define SEND_OK 0
#define SEND_ERROR 1


#define ESP8266_RST GPIO_PIN_9


u8 *ESP8266_GetIPD(u32 timeOut);

void ESP8266_Init(void);

void ESP8266_Clear(void);

void ESP8266_SendData(u8 *data, u32 len);

u8 ESP8266_WaitRecive(void);

u8 ESP8266_SendCmd(char *cmd, char *res);

#endif
