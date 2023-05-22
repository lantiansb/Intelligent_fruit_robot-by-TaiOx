/*
 * @Author: lantiansb lantiansb@foxmail.com
 * @Date: 2023-03-15 15:41:11
 * @LastEditors: lantiansb lantiansb@foxmail.com
 * @LastEditTime: 2023-05-04 15:31:03
 */
#include "ht32.h"
//#include "sys.h"     //使用sys整理所有MCU资源的初始化
#include <stdio.h>
#include <string.h>
#include "delay.h"
#include "usart.h"
#include "Servo.h"
#include "led.h"
#include "K210.h"
#include "MachineArm.h"

/*************************** 变量声明 *************************/
extern unsigned char cmd;//视觉抓取板获取小车板的抓取命令

/**************************实现函数********************************************
函数说明：硬件设备初始化程序
*******************************************************************************/ 
void Dev_Init()
{
/*************************** 初始化板载资源 *************************/
    LED_Init();
    CLOSE_LED();
/*************************** 初始化机械臂 *************************/
    pwm_start();
    servo_reset_begin();//机械臂初始化姿态校正
    USART_Configuration();
    delay_ms(1000);
}


/*************************** 主函数 *************************/
int main(void)
{
    Dev_Init();//硬件初始化
    while(1)
    {
        if(cmd == 0xF8)
        {
            cmd = 0;
            OPEN_LED();
            Eye_Catch("1");
            Usart_Sendbyte(HT_USART1, 0xA5);//表示该次抓取已完成
        }
        else if(cmd == 0xF9)
        {
            cmd = 0;
            OPEN_LED();
            Eye_Catch("2");
            Usart_Sendbyte(HT_USART1, 0xA5);//表示该次抓取已完成
        }
        else
            CLOSE_LED();
    }
    //正常不应该走到这里
    return 0;
}
