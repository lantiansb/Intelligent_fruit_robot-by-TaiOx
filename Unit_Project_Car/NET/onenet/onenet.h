#ifndef _ONENET_H_
#define _ONENET_H_

#include "ht32.h"

#define OK 0
#define NO 1

/************************ 当前版本暂不做管理系统，仅动态创建订单 *************************/
//订单信息结构体
typedef struct Order_Struct
{
	u8* Client_ID;
    
	u8 fruit1_num;		 //水果1个数
	
    u8 fruit2_num;       //水果2个数
	
} Order_t;

unsigned char Check_ONENET(void);

void ONENET_DOWHAT(unsigned char ONENET_sta);

_Bool OneNet_DevLink(void);

void OneNet_SendData(void);

void OneNet_RevPro(unsigned char *cmd);

void OneNet_SendPicture(char *devid, const char* picture, unsigned int pic_len);

u8 SubscribeOrder(void);//订阅主题函数

Order_t* Order_UnPacket(void);//订单解包函数

#endif
