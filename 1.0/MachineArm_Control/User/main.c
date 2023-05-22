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
    
    USART_Configuration();
    printf("hhhh\r\n");
    pwm_start();
    
//    servo_control(0,-45,85,85,0,0);
//    delay_ms(2000);
//    servo_control(0,0,0,0,0,0);
//    delay_ms(2000);
//    servo_angle_calculate(16, 0, -5);
//    servo_control(target_angle_1,target_angle_2,target_angle_3,target_angle_4,0,0);
//    servo_control(0,0,0,0,0,0);
//    delay_ms(2000);
//    
//    servo_control(0,-45,85,85,0,0);
//    delay_ms(2000);
//    servo_control(0,0,0,0,0,0);
//    delay_ms(2000);
//    servo_angle_calculate(20, 0, -5);
//    servo_control(target_angle_1,target_angle_2,target_angle_3,target_angle_4,0,0);
//    servo_control(0,0,0,0,0,0);
//    delay_ms(2000);

//    delay_ms(2000);
//    servo_angle_calculate(15, 0, 0);
//    servo_control(target_angle_1,target_angle_2,target_angle_3,target_angle_4,0,0);
//    delay_ms(2000);
//    servo_angle_calculate(15, 0, -5);
//    servo_control(target_angle_1,target_angle_2,target_angle_3,target_angle_4,0,0);
//    delay_ms(2000);
//    servo_angle_calculate(15, 0, -10);
//    servo_control(target_angle_1,target_angle_2,target_angle_3,target_angle_4,0,0);
//    delay_ms(2000);
    servo_control(0,0,0,0,0,0);
//    //抓取动作
//    servo_control(target_angle_1,target_angle_2,target_angle_3,target_angle_4,0,20);
//    delay_ms(1000);
//    servo_control(target_angle_1,target_angle_2,target_angle_3,target_angle_4,0,20);
//    delay_ms(1000);
//    servo_angle_calculate(-23, 0, 0);
//    servo_control(target_angle_1,target_angle_2,target_angle_3,target_angle_4,0,0);
//    delay_ms(1000);
//    servo_control(0,0,0,0,0,0);
//    delay_ms(1000);
    
    
    while(1);
}
