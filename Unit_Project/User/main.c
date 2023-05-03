/*
 * @Author: lantiansb lantiansb@foxmail.com
 * @Date: 2023-03-15 15:41:11
 * @LastEditors: lantiansb lantiansb@foxmail.com
 * @LastEditTime: 2023-04-02 15:28:15
 */
#include "ht32.h"
//#include "sys.h"     //ʹ��sys��������MCU��Դ�ĳ�ʼ��
#include <stdio.h>
#include <string.h>
#include "delay.h"
#include "usart.h"
#include "esp8266.h"
#include "MqttKit.h"
#include "onenet.h"
#include "Servo.h"
#include "led.h"
#include "K210.h"
#include "Track.h"
#include "OLED.h"
#include "BMP.h"
#include "MachineArm.h"

/*************************** �������� *************************/
u8 order_flag;//0-����״̬��1-����״̬
u8 Car_flag;//0-AGVС��ͣ�ڣ�1-С������ץȡ�ص㣬2-С����������
u8 Arm_flag;//0-��е��ͣ�ڣ�1-��е������
u8 OLED_show_buf[20];//OLED��ʾ�ַ�������

extern unsigned char esp8266_buf[256];
extern unsigned short esp8266_cnt;

/**************************ʵ�ֺ���********************************************
����˵����Ӳ���豸��ʼ������
*******************************************************************************/ 
void Dev_Init()
{
/*************************** ��ʼ��������Դ *************************/
    USART_Configuration();
    LED_Init();
    CLOSE_LED();
/*************************** ��ʼ����ʾ�� *************************/
    OLED_Init();
    OLED_ColorTurn(0);//0������ʾ��1 ��ɫ��ʾ
    OLED_DisplayTurn(0);//0������ʾ 1 ��Ļ��ת��ʾ
    OLED_ShowString(0, 0, (u8 *)"Initial...", 16, 1);
    OLED_Refresh();//�����Դ�
    delay_ms(1000);
/*************************** ��ʼ����· *************************/
    ESP8266_Init();
    OLED_ShowString(0, 17, (u8 *)"WiFi Connect done.", 8, 1);
    OLED_Refresh();//�����Դ�
    while(OneNet_DevLink());
    delay_ms(2000);
    while(SubscribeOrder());
    delay_ms(1000);
    OLED_ShowString(0, 26, (u8 *)"Onenet Server done.", 8, 1);
    OLED_Refresh();//�����Դ�
/*************************** ��ʼ��AGVС�� *************************/
    Track_Init();
    delay_ms(1000);
    OLED_ShowString(0, 26+9, (u8 *)"AGV Car done.", 8, 1);
    OLED_Refresh();//�����Դ�
/*************************** ��ʼ����е�� *************************/
    pwm_start();
    servo_reset_begin();//��е�۳�ʼ����̬У��
    delay_ms(1000);
    OLED_ShowString(0, 26+18, (u8 *)"MachineArm done.", 8, 1);
    OLED_Refresh();//�����Դ�
    delay_ms(500);
/*************************** һЩû��ʲô�õĵ���ʱ *************************/
    OLED_ShowString(0, 26+27, (u8 *)"Initial done! Sys:3", 8, 1);
    OLED_Refresh();//�����Դ�
    delay_ms(1000);
    OLED_ShowString(0, 26+27, (u8 *)"Initial done! Sys:2", 8, 1);
    OLED_Refresh();//�����Դ�
    delay_ms(1000);
    OLED_ShowString(0, 26+27, (u8 *)"Initial done! Sys:1", 8, 1);
    OLED_Refresh();//�����Դ�
    delay_ms(1000);
    OLED_Clear();
    OLED_ShowString(0, 16, (u8 *)"Welcome to TaiOx", 8, 1);
    OLED_ShowString(0, 32, (u8 *)"State:", 8, 1);
    OLED_ShowString(0, 46, (u8 *)"Waiting for Order.", 8, 1);
    OLED_Refresh();//�����Դ�
    delay_ms(2000);
}


/*************************** ������ *************************/
int main(void)
{
    Dev_Init();//Ӳ����ʼ��
    memset(esp8266_buf, 0, sizeof(esp8266_buf));
    esp8266_cnt = 0;
    while(1)
    {
        Order_t *order_temp = (Order_t*)malloc(sizeof(order_temp));//�������սṹ��ָ��
        if(order_temp != NULL)
            order_temp->Client_ID = NULL;
        if(!order_flag)
        {
            OLED_Clear();
            OLED_ShowString(0, 16, (u8 *)"Welcome to TaiOx", 8, 1);
            OLED_ShowString(0, 32, (u8 *)"State:", 8, 1);
            OLED_ShowString(0, 46, (u8 *)"Waiting for Order.", 8, 1);
            OLED_Refresh();//�����Դ�
            delay_ms(2000);
            order_flag = OLED_ScrollDisplay(8,4,1);//�ڸú����У���WiFi���ڽ��յ����ݾͽ����жϲ�������Ļ����
        }
        if(order_temp != NULL)
            order_temp->Client_ID = NULL;
        OLED_Clear();
        OLED_ShowString(0, 0, (u8 *)"Here is a Order!", 8, 1);//��ʾ���յ�����
        OLED_Refresh();//�����Դ�
        //���ݻ�ȡ
        while(order_temp->Client_ID == NULL)
            order_temp = Order_UnPacket();
        if(order_temp->Client_ID != NULL)//�ٴ�ȷ�϶����û���Ϊ��
        {
            //��ʾ����ʾ������Ϣ��
            sprintf((char *)OLED_show_buf, "boss:%s", order_temp->Client_ID);
            OLED_ShowString(0, 16, OLED_show_buf, 8, 1);
            memset(OLED_show_buf, 0, sizeof(OLED_show_buf));
            sprintf((char *)OLED_show_buf, "Orange:%d", order_temp->fruit1_num);
            OLED_ShowString(0, 32, OLED_show_buf, 8, 1);
            memset(OLED_show_buf, 0, sizeof(OLED_show_buf));
            sprintf((char *)OLED_show_buf, "GreenOrange:%d", order_temp->fruit2_num);
            OLED_ShowString(0, 48, OLED_show_buf, 8, 1);
            memset(OLED_show_buf, 0, sizeof(OLED_show_buf));
            OLED_Refresh();//�����Դ�
            while(order_temp->fruit1_num || order_temp->fruit2_num)
            {
                if(order_temp->fruit1_num)
                {
                    Eye_Catch(1);
                    order_temp->fruit1_num--;
                }
                else if(order_temp->fruit2_num)
                {
                    Eye_Catch(2);
                    order_temp->fruit2_num--;
                }
            }
            order_flag = 0;
        }
        delay_ms(1000);
    }
    return 0;
}
