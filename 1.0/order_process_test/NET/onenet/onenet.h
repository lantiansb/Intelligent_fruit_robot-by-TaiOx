#ifndef _ONENET_H_
#define _ONENET_H_

#define OK 0
#define NO 1

//������Ϣ�ṹ��
typedef struct Order_Buffer
{
	u8 Client_ID;
    
	u8 fruit_type;		//ˮ����������
	
    u8 fruit_num;       //ˮ����������
	
} Order_PACKET_STRUCTURE;

unsigned char Check_ONENET(void);

void ONENET_DOWHAT(unsigned char ONENET_sta);

_Bool OneNet_DevLink(void);

void OneNet_SendData(void);

void OneNet_RevPro(unsigned char *cmd);

void OneNet_SendPicture(char *devid, const char* picture, unsigned int pic_len);

u8 SubscribeOrder(void);//�������⺯��

void Order_Publish(void);//�������⺯������

void Order_UnPacket(void);

#endif
