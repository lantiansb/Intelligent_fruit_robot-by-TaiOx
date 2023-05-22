#ifndef _ONENET_H_
#define _ONENET_H_

#include "ht32.h"

#define OK 0
#define NO 1

/************************ ��ǰ�汾�ݲ�������ϵͳ������̬�������� *************************/
//������Ϣ�ṹ��
typedef struct Order_Struct
{
	u8* Client_ID;
    
	u8 fruit1_num;		 //ˮ��1����
	
    u8 fruit2_num;       //ˮ��2����
	
} Order_t;

unsigned char Check_ONENET(void);

void ONENET_DOWHAT(unsigned char ONENET_sta);

_Bool OneNet_DevLink(void);

void OneNet_SendData(void);

void OneNet_RevPro(unsigned char *cmd);

void OneNet_SendPicture(char *devid, const char* picture, unsigned int pic_len);

u8 SubscribeOrder(void);//�������⺯��

Order_t* Order_UnPacket(void);//�����������

#endif
