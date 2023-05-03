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

int main(void)
{
    USART_Configuration();
    ESP8266_Init();
    while(OneNet_DevLink());
    delay_ms(2000);
    while(SubscribeOrder());
    
    while(1)
    {
        
/*************************** 接收数据处理 *************************/
        Order_UnPacket();
        delay_ms(1000);
    }
}
