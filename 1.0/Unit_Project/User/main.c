/*
 * @Author: lantiansb lantiansb@foxmail.com
 * @Date: 2023-03-15 15:41:11
 * @LastEditors: lantiansb lantiansb@foxmail.com
 * @LastEditTime: 2023-04-02 15:28:15
 */
#include "ht32.h"
//#include "sys.h"     //使用sys整理所有MCU资源的初始化
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

/*************************** 变量声明 *************************/
u8 order_flag;//0-空闲状态，1-订单状态
u8 Car_flag;//0-AGV小车停摆，1-小车到达抓取地点，2-小车到达打包点
u8 Arm_flag;//0-机械臂停摆，1-机械臂运行
u8 OLED_show_buf[20];//OLED显示字符串缓存

float location_xyz[3];//当前抓取的目标的世界坐标

//从servo.c中调过来的机械臂位姿参数
extern double target_angle_1;
extern double target_angle_2;
extern double target_angle_3;
extern double target_angle_4;

extern unsigned char esp8266_buf[256];
extern unsigned short esp8266_cnt;
u8 fruit_num[2];
/**************************实现函数********************************************
函数说明：硬件设备初始化程序
*******************************************************************************/ 
void Dev_Init()
{
/*************************** 初始化板载资源 *************************/
    USART_Configuration();
    LED_Init();
    CLOSE_LED();
/*************************** 初始化显示屏 *************************/
    OLED_Init();
    OLED_ColorTurn(0);//0正常显示，1 反色显示
    OLED_DisplayTurn(0);//0正常显示 1 屏幕翻转显示
    OLED_ShowString(0, 0, (u8 *)"Initial...", 16, 1);
    OLED_Refresh();//更新显存
    delay_ms(1000);
/*************************** 初始化网路 *************************/
    ESP8266_Init();
    OLED_ShowString(0, 17, (u8 *)"WiFi Connect done.", 8, 1);
    OLED_Refresh();//更新显存
    while(OneNet_DevLink());
    delay_ms(2000);
    while(SubscribeOrder());
    delay_ms(1000);
    OLED_ShowString(0, 26, (u8 *)"Onenet Server done.", 8, 1);
    OLED_Refresh();//更新显存
/*************************** 初始化AGV小车 *************************/
    Track_Init();
    OLED_ShowString(0, 26+9, (u8 *)"AGV Car done.", 8, 1);
    OLED_Refresh();//更新显存
    delay_ms(1000);
/*************************** 初始化机械臂 *************************/
    pwm_start();
    servo_reset_begin();//机械臂初始化姿态校正
    OLED_ShowString(0, 26+18, (u8 *)"MachineArm done.", 8, 1);
    OLED_Refresh();//更新显存
    delay_ms(1000);
/*************************** 一些没有什么用的倒计时 *************************/
    OLED_ShowString(0, 26+27, (u8 *)"Initial done! Sys:3", 8, 1);
    OLED_Refresh();//更新显存
    delay_ms(1000);
    OLED_ShowString(0, 26+27, (u8 *)"Initial done! Sys:2", 8, 1);
    OLED_Refresh();//更新显存
    delay_ms(1000);
    OLED_ShowString(0, 26+27, (u8 *)"Initial done! Sys:1", 8, 1);
    OLED_Refresh();//更新显存
    delay_ms(1000);
    OLED_Clear();
    OLED_ShowString(0, 16, (u8 *)"Welcome to TaiOx", 8, 1);
    OLED_ShowString(0, 32, (u8 *)"State:", 8, 1);
    OLED_ShowString(0, 46, (u8 *)"Waiting for Order.", 8, 1);
    OLED_Refresh();//更新显存
    delay_ms(2000);
}


/*************************** 主函数 *************************/
int main(void)
{
    Dev_Init();//硬件初始化
    memset(esp8266_buf, 0, sizeof(esp8266_buf));
    esp8266_cnt = 0;
    while(1)
    {
        Order_t *order_temp; //局部订单结构体
        order_temp = (Order_t*)malloc(sizeof(order_temp));//订单接收结构体指针
        if(order_temp != NULL)
            order_temp->Client_ID = NULL;
        if(!order_flag)
        {
            OLED_Clear();
            OLED_ShowString(0, 16, (u8 *)"Welcome to TaiOx", 8, 1);
            OLED_ShowString(0, 32, (u8 *)"State:", 8, 1);
            OLED_ShowString(0, 46, (u8 *)"Waiting for Order.", 8, 1);
            OLED_Refresh();//更新显存
            delay_ms(2000);
            order_flag = OLED_ScrollDisplay(8,4,1);//在该函数中，若WiFi串口接收到数据就进入中断并跳出屏幕滚动
        }
        if(order_temp != NULL)
            order_temp->Client_ID = NULL;
        OLED_Clear();
        OLED_ShowString(0, 0, (u8 *)"Here is a Order!", 8, 1);//表示接收到订单
        OLED_Refresh();//更新显存
        //数据获取
        while(order_temp->Client_ID == NULL)
            order_temp = Order_UnPacket();
        if(order_temp->Client_ID != NULL)//再次确认订单用户不为零
        {
            //显示屏显示订单信息！
            sprintf((char *)OLED_show_buf, "boss:%s", order_temp->Client_ID);
            OLED_ShowString(0, 16, OLED_show_buf, 8, 1);
            memset(OLED_show_buf, 0, sizeof(OLED_show_buf));
            sprintf((char *)OLED_show_buf, "Orange:%d", order_temp->fruit1_num);
            OLED_ShowString(0, 32, OLED_show_buf, 8, 1);
            memset(OLED_show_buf, 0, sizeof(OLED_show_buf));
            sprintf((char *)OLED_show_buf, "GreenOrange:%d", order_temp->fruit2_num);
            OLED_ShowString(0, 48, OLED_show_buf, 8, 1);
            memset(OLED_show_buf, 0, sizeof(OLED_show_buf));
            OLED_Refresh();//更新显存
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
                    delay_ms(100);//等待K210返回坐标
                    location_xyz_cpy = K210_GetIPD(200);//对目标坐标进行赋值
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
                    delay_ms(100);//等待K210返回坐标
                    location_xyz_cpy = K210_GetIPD(200);//对目标坐标进行赋值
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
