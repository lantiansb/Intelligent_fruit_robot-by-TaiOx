/*
 * @Author: lantiansb lantiansb@foxmail.com
 * @Date: 2023-03-15 15:41:11
 * @LastEditors: lantiansb lantiansb@foxmail.com
 * @LastEditTime: 2023-04-02 15:28:15
 */
#include "ht32.h"
#include <stdio.h>
#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "esp8266.h"
#include "onenet.h"

int main(void)
{
    USART_Configuration();
    ESP8266_Init();
    OneNet_DevLink();
    while(1)
    {
        delay_ms(2000);
        printf( "OneNet_SendData\r\n");//通过串口1发送提示信息（要开始发送数据了）
            
        OneNet_SendData();//发送数据给onenet

        printf("send_data_end\n");

        ESP8266_Clear();
    }
}
