#ifndef _ONENET_H_
#define _ONENET_H_

#define OK 0
#define NO 1

//订单信息结构体
typedef struct Order_Buffer
{
	u8 Client_ID;
    
	u8 fruit_type;		//水果种类数组
	
    u8 fruit_num;       //水果个数数组
	
} Order_PACKET_STRUCTURE;

unsigned char Check_ONENET(void);

void ONENET_DOWHAT(unsigned char ONENET_sta);

_Bool OneNet_DevLink(void);

void OneNet_SendData(void);

void OneNet_RevPro(unsigned char *cmd);

void OneNet_SendPicture(char *devid, const char* picture, unsigned int pic_len);

u8 SubscribeOrder(void);//订阅主题函数

void Order_Publish(void);//发布主题函数测试

void Order_UnPacket(void);

#endif
