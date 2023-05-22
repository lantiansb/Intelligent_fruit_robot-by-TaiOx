/*
 * @Author: lantiansb lantiansb@foxmail.com
 * @Date: 2023-03-15 15:41:11
 * @LastEditors: lantiansb lantiansb@foxmail.com
 * @LastEditTime: 2023-05-04 15:31:03
 */
#include "ht32.h"
//#include "sys.h"     //ʹ��sys��������MCU��Դ�ĳ�ʼ��
#include <stdio.h>
#include <string.h>
#include "delay.h"
#include "usart.h"
#include "Servo.h"
#include "led.h"
#include "K210.h"
#include "MachineArm.h"

/*************************** �������� *************************/
extern unsigned char cmd;//�Ӿ�ץȡ���ȡС�����ץȡ����

/**************************ʵ�ֺ���********************************************
����˵����Ӳ���豸��ʼ������
*******************************************************************************/ 
void Dev_Init()
{
/*************************** ��ʼ��������Դ *************************/
    LED_Init();
    CLOSE_LED();
/*************************** ��ʼ����е�� *************************/
    pwm_start();
    servo_reset_begin();//��е�۳�ʼ����̬У��
    USART_Configuration();
    delay_ms(1000);
}


/*************************** ������ *************************/
int main(void)
{
    Dev_Init();//Ӳ����ʼ��
    while(1)
    {
        if(cmd == 0xF8)
        {
            cmd = 0;
            OPEN_LED();
            Eye_Catch("1");
            Usart_Sendbyte(HT_USART1, 0xA5);//��ʾ�ô�ץȡ�����
        }
        else if(cmd == 0xF9)
        {
            cmd = 0;
            OPEN_LED();
            Eye_Catch("2");
            Usart_Sendbyte(HT_USART1, 0xA5);//��ʾ�ô�ץȡ�����
        }
        else
            CLOSE_LED();
    }
    //������Ӧ���ߵ�����
    return 0;
}
