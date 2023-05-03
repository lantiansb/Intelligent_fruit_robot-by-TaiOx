//单片机头文件
#include "HT32.h"

//网络设备
#include "esp8266.h"

//协议文件
#include "onenet.h"
#include "mqttkit.h"

//硬件驱动
#include "usart.h"
#include "delay.h"
#include "time.h"


//json库
#include "cjson.h"

//C库
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define PROID		"592489"

#define AUTH_INFO	"12345678"

#define DEVID		"1072081358"

const int8 *topics[] = {"/deviceA/commands"};//发送订单主题

//==========================================================
//	函数名称：	OneNet_DevLink
//
//	函数功能：	与onenet创建连接
//
//	入口参数：	无
//
//	返回参数：	0-成功
//
//	说明：		与onenet平台建立连接
//==========================================================
_Bool OneNet_DevLink(void)
{
	
	MQTT_PACKET_STRUCTURE mqttPacket = {NULL, 0, 0, 0};					//协议包

	unsigned char *dataPtr;
	
	_Bool status = 1;
	//打印一下信息产品id,鉴权信息，设备ID
    printf("OneNet_DevLink\r\nPROID: %s,	AUIF: %s,	DEVID:%s\r\n", PROID, AUTH_INFO, DEVID);
	
	if(MQTT_PacketConnect(PROID, AUTH_INFO, DEVID, 256, 0, MQTT_QOS_LEVEL0, NULL, NULL, 0, &mqttPacket) == 0)
	{
		ESP8266_SendData(mqttPacket._data, mqttPacket._len);			//上传平台
		
		dataPtr = ESP8266_GetIPD(250);									//等待平台响应
		if(dataPtr != NULL)//如果平台返回数据不为空则
		{
			if(MQTT_UnPacketRecv(dataPtr) == MQTT_PKT_CONNACK)//	MQTT数据接收类型判断（connack报文）
			{
				switch(MQTT_UnPacketConnectAck(dataPtr))//打印是否连接成功及连接失败的原因
				{
					case 0:printf( "Tips:	连接成功\r\n");status = 0;break;
					
					case 1:printf(  "WARN:	连接失败：协议错误\r\n");break;
					case 2:printf(  "WARN:	连接失败：非法的clientid\r\n");break;
					case 3:printf(  "WARN:	连接失败：服务器失败\r\n");break;
					case 4:printf(  "WARN:	连接失败：用户名或密码错误\r\n");break;
					case 5:printf(  "WARN:	连接失败：非法链接(比如token非法)\r\n");break;
					
					default:printf(  "ERR:	连接失败：未知错误\r\n");break;
				}
			}
		}
		
		MQTT_DeleteBuffer(&mqttPacket);								//删包
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
//	函数名称：	OneNet_SendData
//
//	函数功能：	上传数据到平台
//
//	入口参数：	type：发送数据的格式
//
//	返回参数：	无
//
//	说明：		
//==========================================================
void OneNet_SendData(void)
{
	
	MQTT_PACKET_STRUCTURE mqttPacket = {NULL, 0, 0, 0};												//协议包
	
	char buf[128];
	
	short body_len = 0;
	
	printf( "Tips:	OneNet_SendData-MQTT\r\n");
	
	memset(buf, 0, sizeof(buf));//清空数组内容
		
	if(body_len)
	{
		if(MQTT_PacketSaveData(DEVID, body_len, NULL, 5, &mqttPacket) == 0)							//封包
		{
			
			ESP8266_SendData(mqttPacket._data, mqttPacket._len);									//上传数据到平台
			printf( "Send %d Bytes\r\n", mqttPacket._len);
			
			MQTT_DeleteBuffer(&mqttPacket);															//删包
		}
		else
			printf(  "WARN:	EDP_NewBuffer Failed\r\n");
	}
	
}

//==========================================================
//	函数名称：	SubscribeOrder
//
//	函数功能：	订阅下单主题
//
//	入口参数：	type：发送数据的格式
//
//	返回参数：	返回订阅是否成功
//
//	说明：		
//==========================================================
u8 SubscribeOrder(void)
{
	MQTT_PACKET_STRUCTURE mqttPacket = {NULL, 0, 0, 0};												//协议包
	printf( "Tips:	OneNet_SubscribeOrder\r\n");
    
    if(MQTT_PacketSubscribe(MQTT_SUBSCRIBE_ID, MQTT_QOS_LEVEL0, topics, sizeof(topics)/4, &mqttPacket) == OK)
    {
        ESP8266_SendData(mqttPacket._data, mqttPacket._len);									//上传数据到平台
        printf( "Send %d Bytes\r\n", mqttPacket._len);
        printf( "%d\r\n", mqttPacket._memFlag);
        printf( "%d\r\n", mqttPacket._size);
        MQTT_DeleteBuffer(&mqttPacket);															//删包
        return OK;
    }
    else
        printf(  "WARN:	Subscribe Failed\r\n");
    return NO;
}

//==========================================================
//	函数名称：	Order_UnPacket
//
//	函数功能：	订单信息解包
//
//	入口参数：	str: 8266接收到的订单负载
//
//	返回参数：	无
//
//	说明：		
//==========================================================
Order_t* Order_UnPacket(void)
{
    Order_t *order_temp;
    unsigned char Order_buf[128];//订单缓存数组
    unsigned char *dataPtr = NULL;
    int8 *topic;
    uint16 topic_len;
    int8 *payload_rec;
    uint16 payload_len;
    uint8 qos;
    uint16 pkt_id;
    //MQTT信息解包获取有效载荷
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
    
    if(dataPtr != NULL)//如果返回数据不为空
    {
        uint8 result = MQTT_UnPacketPublish(dataPtr, &topic, &topic_len, &payload_rec, &payload_len, &qos, &pkt_id);
        //payload_rec: 应为"ID:lantian;type:1,num:3;type:2,num:2"表示水果1要3个，水果2要2个
        
        if (result == 0)    // 使用提取出的值
        {
            free(dataPtr);
//            printf("接收到的 PUBLISH 数据包主题为：%.*s，负载为：%.*s\r\n", topic_len, topic, payload_len, payload_rec);
            strcpy((char *)Order_buf, payload_rec);
            //订单信息解包过程
            /* 获取用户ID */
            //ID:13827500560;type1,num:2;type2,num:3切割
            token = strtok_r((char *)Order_buf, s, &next_token);
            strtok_r((char *)token, s3, &next_ID_token);
            //ID:13827500560切割
            ID_token = strtok_r(NULL, s3, &next_ID_token);
            order_temp->Client_ID = (unsigned char *)ID_token;
            
            /* 获取水果数量1 */
            //type1,num:2;type2,num:3切割
            token = strtok_r(NULL, s, &next_token);
            //type:1,num:2切割
            strtok_r(token, s2, &next_token2);
            token2 = strtok_r(NULL, s2, &next_token2);
            //num:2切割
            strtok_r(token2, s3, &next_num_token);
            num_token = strtok_r(NULL, s3, &next_num_token);
            order_temp->fruit1_num = atoi(num_token);//水果1数量获取
            
            /* 获取水果数量2 */
            //type1,num:2;type2,num:3切割
            token = strtok_r(NULL, s, &next_token);
            //type:2,num：3切割
            strtok_r(token, s2, &next_token2);
            token2 = strtok_r(NULL, s2, &next_token2);
            //num:2切割
            strtok_r(token2, s3, &next_num_token);
            num_token = strtok_r(NULL, s3, &next_num_token);
            order_temp->fruit2_num = atoi(num_token);//水果2数量获取
            
            //释放掉所用的空间，防止溢出，并指向NULL
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
            printf("解包失败\r\n");
        }
        ESP8266_Clear();
    }
    return order_temp;
}
