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

/*************************** �������� *************************/
u8 order_flag;//0-����״̬��1-����״̬
u8 Car_flag;//0-AGVС��ͣ�ڣ�1-С������ץȡ�ص㣬2-С����������
u8 Arm_flag;//0-��е��ͣ�ڣ�1-��е������
u8 OLED_show_buf[20];//OLED��ʾ�ַ�������

float location_xyz[3];//��ǰץȡ��Ŀ�����������

//��servo.c�е������Ļ�е��λ�˲���
extern double target_angle_1;
extern double target_angle_2;
extern double target_angle_3;
extern double target_angle_4;

extern unsigned char esp8266_buf[256];
extern unsigned short esp8266_cnt;
u8 fruit_num[2];
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
    OLED_ShowString(0, 26+9, (u8 *)"AGV Car done.", 8, 1);
    OLED_Refresh();//�����Դ�
    delay_ms(1000);
/*************************** ��ʼ����е�� *************************/
    pwm_start();
    servo_reset_begin();//��е�۳�ʼ����̬У��
    OLED_ShowString(0, 26+18, (u8 *)"MachineArm done.", 8, 1);
    OLED_Refresh();//�����Դ�
    delay_ms(1000);
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
        Order_t *order_temp; //�ֲ������ṹ��
        order_temp = (Order_t*)malloc(sizeof(order_temp));//�������սṹ��ָ��
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
            fruit_num[0] = order_temp->fruit1_num;
            fruit_num[1] = order_temp->fruit2_num;
            while(order_temp->fruit1_num || order_temp->fruit2_num)
            {
                if(fruit_num[0])
                {
                    servo_control(0,0,0,134,0,0);
                    float *location_xyz_cpy = NULL;
                    delay_ms(1000);
                    K210_SendCmd("1");
                    delay_ms(100);//�ȴ�K210��������
                    location_xyz_cpy = K210_GetIPD(200);//��Ŀ��������и�ֵ
                    if(location_xyz_cpy != NULL)
                    {
                        location_xyz[0] = *location_xyz_cpy++;
                        location_xyz[1] = *location_xyz_cpy++;
                        location_xyz[2] = *location_xyz_cpy;
                    }
                    servo_angle_calculate(location_xyz[0],location_xyz[1],location_xyz[2]);
                    servo_catch();
                    fruit_num[0]--;
                }
                else if(fruit_num[1])
                {
                    servo_control(0,0,0,134,0,0);
                    float *location_xyz_cpy = NULL;
                    delay_ms(1000);
                    K210_SendCmd("2");
                    delay_ms(100);//�ȴ�K210��������
                    location_xyz_cpy = K210_GetIPD(200);//��Ŀ��������и�ֵ
                    if(location_xyz_cpy != NULL)
                    {
                        location_xyz[0] = *location_xyz_cpy++;
                        location_xyz[1] = *location_xyz_cpy++;
                        location_xyz[2] = *location_xyz_cpy;
                    }
                    servo_angle_calculate(location_xyz[0],location_xyz[1],location_xyz[2]);
                    servo_catch();
                    fruit_num[1]--;
                }
            }
            order_flag = 0;
            free(order_temp);
            order_temp = NULL;
        }
        delay_ms(1000);
    }
    return 0;
}
