//��Ƭ��ͷ�ļ�
#include "HT32.h"

//�����豸
#include "esp8266.h"

//Э���ļ�
#include "onenet.h"
#include "mqttkit.h"

//Ӳ������
#include "usart.h"
#include "delay.h"
#include "time.h"


//json��
#include "cjson.h"

//C��
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define PROID		"592489"

#define AUTH_INFO	"12345678"

#define DEVID		"1072081358"

const int8 *topics[] = {"/deviceA/commands"};//���Ͷ�������

//==========================================================
//	�������ƣ�	OneNet_DevLink
//
//	�������ܣ�	��onenet��������
//
//	��ڲ�����	��
//
//	���ز�����	0-�ɹ�
//
//	˵����		��onenetƽ̨��������
//==========================================================
_Bool OneNet_DevLink(void)
{
	
	MQTT_PACKET_STRUCTURE mqttPacket = {NULL, 0, 0, 0};					//Э���

	unsigned char *dataPtr;
	
	_Bool status = 1;
	//��ӡһ����Ϣ��Ʒid,��Ȩ��Ϣ���豸ID
    printf("OneNet_DevLink\r\nPROID: %s,	AUIF: %s,	DEVID:%s\r\n", PROID, AUTH_INFO, DEVID);
	
	if(MQTT_PacketConnect(PROID, AUTH_INFO, DEVID, 256, 0, MQTT_QOS_LEVEL0, NULL, NULL, 0, &mqttPacket) == 0)
	{
		ESP8266_SendData(mqttPacket._data, mqttPacket._len);			//�ϴ�ƽ̨
		
		dataPtr = ESP8266_GetIPD(250);									//�ȴ�ƽ̨��Ӧ
		if(dataPtr != NULL)//���ƽ̨�������ݲ�Ϊ����
		{
			if(MQTT_UnPacketRecv(dataPtr) == MQTT_PKT_CONNACK)//	MQTT���ݽ��������жϣ�connack���ģ�
			{
				switch(MQTT_UnPacketConnectAck(dataPtr))//��ӡ�Ƿ����ӳɹ�������ʧ�ܵ�ԭ��
				{
					case 0:printf( "Tips:	���ӳɹ�\r\n");status = 0;break;
					
					case 1:printf(  "WARN:	����ʧ�ܣ�Э�����\r\n");break;
					case 2:printf(  "WARN:	����ʧ�ܣ��Ƿ���clientid\r\n");break;
					case 3:printf(  "WARN:	����ʧ�ܣ�������ʧ��\r\n");break;
					case 4:printf(  "WARN:	����ʧ�ܣ��û������������\r\n");break;
					case 5:printf(  "WARN:	����ʧ�ܣ��Ƿ�����(����token�Ƿ�)\r\n");break;
					
					default:printf(  "ERR:	����ʧ�ܣ�δ֪����\r\n");break;
				}
			}
		}
		
		MQTT_DeleteBuffer(&mqttPacket);								//ɾ��
	}
	else
		printf( "WARN:	MQTT_PacketConnect Failed\r\n");
	
	return status;
	
}

unsigned char OneNet_FillBuf(char *buf)
{
	
	char text[32];
	memset(text, 0, sizeof(text));
	
	strcpy(buf, ",;");
	
	memset(text, 0, sizeof(text));
	sprintf(text, "Temperature,%d.%d;",rand()%25,rand()%25);
	strcat(buf, text);
	
//	memset(text, 0, sizeof(text));
//	sprintf(text, "Humidity,%d.%d;", 35,29);
//	strcat(buf, text);

	printf("buf_mqtt=%s\r\n",buf);
	return strlen(buf);

}

//==========================================================
//	�������ƣ�	OneNet_SendData
//
//	�������ܣ�	�ϴ����ݵ�ƽ̨
//
//	��ڲ�����	type���������ݵĸ�ʽ
//
//	���ز�����	��
//
//	˵����		
//==========================================================
void OneNet_SendData(void)
{
	
	MQTT_PACKET_STRUCTURE mqttPacket = {NULL, 0, 0, 0};												//Э���
	
	char buf[128];
	
	short body_len = 0;
	
	printf( "Tips:	OneNet_SendData-MQTT\r\n");
	
	memset(buf, 0, sizeof(buf));//�����������
		
	if(body_len)
	{
		if(MQTT_PacketSaveData(DEVID, body_len, NULL, 5, &mqttPacket) == 0)							//���
		{
			
			ESP8266_SendData(mqttPacket._data, mqttPacket._len);									//�ϴ����ݵ�ƽ̨
			printf( "Send %d Bytes\r\n", mqttPacket._len);
			
			MQTT_DeleteBuffer(&mqttPacket);															//ɾ��
		}
		else
			printf(  "WARN:	EDP_NewBuffer Failed\r\n");
	}
	
}

//==========================================================
//	�������ƣ�	SubscribeOrder
//
//	�������ܣ�	�����µ�����
//
//	��ڲ�����	type���������ݵĸ�ʽ
//
//	���ز�����	���ض����Ƿ�ɹ�
//
//	˵����		
//==========================================================
u8 SubscribeOrder(void)
{
	MQTT_PACKET_STRUCTURE mqttPacket = {NULL, 0, 0, 0};												//Э���
	printf( "Tips:	OneNet_SubscribeOrder\r\n");
    
    if(MQTT_PacketSubscribe(MQTT_SUBSCRIBE_ID, MQTT_QOS_LEVEL0, topics, sizeof(topics)/4, &mqttPacket) == OK)
    {
        ESP8266_SendData(mqttPacket._data, mqttPacket._len);									//�ϴ����ݵ�ƽ̨
        printf( "Send %d Bytes\r\n", mqttPacket._len);
        printf( "%d\r\n", mqttPacket._memFlag);
        printf( "%d\r\n", mqttPacket._size);
        MQTT_DeleteBuffer(&mqttPacket);															//ɾ��
        return OK;
    }
    else
        printf(  "WARN:	Subscribe Failed\r\n");
    return NO;
}

//==========================================================
//	�������ƣ�	Order_UnPacket
//
//	�������ܣ�	������Ϣ���
//
//	��ڲ�����	str: 8266���յ��Ķ�������
//
//	���ز�����	��
//
//	˵����		
//==========================================================
Order_t* Order_UnPacket(void)
{
    Order_t *order_temp;
    unsigned char Order_buf[128];//������������
    unsigned char *dataPtr = NULL;
    int8 *topic;
    uint16 topic_len;
    int8 *payload_rec;
    uint16 payload_len;
    uint8 qos;
    uint16 pkt_id;
    //MQTT��Ϣ�����ȡ��Ч�غ�
    dataPtr = ESP8266_GetIPD(250);
    const char s[2] = ";";
    char* token;
    char* next_token = NULL;
    
    char* ID_token;
    char* next_ID_token = NULL;

    const char s2[2] = ",";
    char* token2;
    char* next_token2 = NULL;
    
    char* num_token;
    char* next_num_token = NULL;
    
    const char s3[2] = ":";
    
    if(dataPtr != NULL)//����������ݲ�Ϊ��
    {
        uint8 result = MQTT_UnPacketPublish(dataPtr, &topic, &topic_len, &payload_rec, &payload_len, &qos, &pkt_id);
        //payload_rec: ӦΪ"ID:lantian;type:1,num:3;type:2,num:2"��ʾˮ��1Ҫ3����ˮ��2Ҫ2��
        
        if (result == 0)    // ʹ����ȡ����ֵ
        {
            free(dataPtr);
//            printf("���յ��� PUBLISH ���ݰ�����Ϊ��%.*s������Ϊ��%.*s\r\n", topic_len, topic, payload_len, payload_rec);
            strcpy((char *)Order_buf, payload_rec);
            //������Ϣ�������
            /* ��ȡ�û�ID */
            //ID:13827500560;type1,num:2;type2,num:3�и�
            token = strtok_r((char *)Order_buf, s, &next_token);
            strtok_r((char *)token, s3, &next_ID_token);
            //ID:13827500560�и�
            ID_token = strtok_r(NULL, s3, &next_ID_token);
            order_temp->Client_ID = (unsigned char *)ID_token;
            
            /* ��ȡˮ������1 */
            //type1,num:2;type2,num:3�и�
            token = strtok_r(NULL, s, &next_token);
            //type:1,num:2�и�
            strtok_r(token, s2, &next_token2);
            token2 = strtok_r(NULL, s2, &next_token2);
            //num:2�и�
            strtok_r(token2, s3, &next_num_token);
            num_token = strtok_r(NULL, s3, &next_num_token);
            order_temp->fruit1_num = atoi(num_token);//ˮ��1������ȡ
            
            /* ��ȡˮ������2 */
            //type1,num:2;type2,num:3�и�
            token = strtok_r(NULL, s, &next_token);
            //type:2,num��3�и�
            strtok_r(token, s2, &next_token2);
            token2 = strtok_r(NULL, s2, &next_token2);
            //num:2�и�
            strtok_r(token2, s3, &next_num_token);
            num_token = strtok_r(NULL, s3, &next_num_token);
            order_temp->fruit2_num = atoi(num_token);//ˮ��2������ȡ
            
            //�ͷŵ����õĿռ䣬��ֹ�������ָ��NULL
            free(order_temp);
            order_temp = NULL;
            free(Order_buf);
            free(topic);
            topic = NULL;
            free(payload_rec);
            payload_rec = NULL;
        }
        else if(result)
        {
            printf("���ʧ��\r\n");
        }
        ESP8266_Clear();
    }
    return order_temp;
}
