/*
 * @Author: lantiansb lantiansb@foxmail.com
 * @Date: 2023-03-15 15:41:11
 * @LastEditors: lantiansb lantiansb@foxmail.com
 * @LastEditTime: 2023-04-02 15:28:15
 */
#include "ht32.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "servo.h"

extern double target_angle_1;
extern double target_angle_2;
extern double target_angle_3;
extern double target_angle_4;

u8 rec_buf[20];
u8 rec_index;
u8 rec_size;

int main(void)
{
    memset(rec_buf, 0, sizeof(rec_buf));
    
    pwm_start();
//    USART_Configuration();
//    printf("hhhh\r\n");
    servo_angle_calculate(23, 0, 0);
    
    servo_control(target_angle_1,-target_angle_2,target_angle_3,target_angle_4,0,0);
    delay_ms(1000);
    servo_control(target_angle_1,-target_angle_2,target_angle_3,target_angle_4,0,20);
    servo_angle_calculate(-23, 0, 0);
    servo_control(target_angle_1,-target_angle_2,target_angle_3,target_angle_4,0,0);
    delay_ms(1000);
    servo_control(0,-0,0,0,0,0);
//    servo_control(0,-53.000000,-9.684918,90.185072,0,0);
//    servo_control(0,-53.000000,-9.684918,90.185072,0,60);
//    servo_control(0,-0,0,0,0,90);
//    servo_control(0,53.000000,9.684918,-90.185072,0,60);
//    servo_control(0,53.000000,9.684918,-90.185072,0,0);
//    servo_control(0,-0,0,0,0,0);   
    
    while(1)
    {
        
//        sscanf((char *)rec_buf, "%d %d %d", &location[0], &location[1], &location[2]);
//        printf("%d %d %d\r\n", location[0], location[1], location[2]);
        
//        servo_control(target_angle_1,target_angle_2,target_angle_3,target_angle_4,0,0);
//        servo_control(0,-45,0,0,0,0);
//        servo_control(0,0,45,0,0,0);
//        servo_control(0,0,0,45,0,0);
//        servo_control(0,0,0,0,45,0);
//        servo_control(0,0,0,0,0,45);
//        delay_ms(5000);
//        printf("Loading Finished!\r\n");
//        delay_ms(1000);	
//        servo_angle_calculate(13, 20, 10);
//        delay_ms(1000);
//        servo_control(0, 0, 0, 0, 45, 0);
//        delay_ms(1000);
//        servo_control(0, 0, 0, 0, 45, 0);
//        delay_ms(1000);
//        delay_ms(1000);
    }
}
