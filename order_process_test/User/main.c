/*
 * @Author: lantiansb lantiansb@foxmail.com
 * @Date: 2023-03-15 15:41:11
 * @LastEditors: lantiansb lantiansb@foxmail.com
 * @LastEditTime: 2023-04-02 15:28:15
 */
#include "ht32.h"
#include <stdio.h>
#include <string.h>
#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "esp8266.h"
#include "MqttKit.h"
#include "onenet.h"

//uint8 received_data[256];

int main(void)
{
	unsigned char *dataPtr = NULL;
    USART_Configuration();
    ESP8266_Init();
    while(OneNet_DevLink());
    delay_ms(2000);
    while(SubscribeOrder());
//    Order_Publish();
    while(1)
    {
        Order_Publish();
        int8 *topic;
        uint16 topic_len;
        int8 *payload_rec;
        uint16 payload_len;
        uint8 qos;
        uint16 pkt_id;
        dataPtr = ESP8266_GetIPD(0);
		if(dataPtr != NULL)//如果返回数据不为空
        {
            uint8 result = MQTT_UnPacketPublish(dataPtr, &topic, &topic_len, &payload_rec, &payload_len, &qos, &pkt_id);
            if (result == 0)    // 使用提取出的值
            {
                printf("接收到的 PUBLISH 数据包主题为：%.*s，负载为：%.*s\r\n", topic_len, topic, payload_len, payload_rec);
                memset(dataPtr, 0, sizeof((unsigned char*)dataPtr));
            }
            else if(result == 0)
                printf("解包失败\r\n");
        }
        delay_ms(2000);
    }
}
