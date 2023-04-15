/*
 * @Author: lantiansb lantiansb@foxmail.com
 * @Date: 2023-03-17 17:21:15
 * @LastEditors: lantiansb lantiansb@foxmail.com
 * @LastEditTime: 2023-04-01 23:28:28
 */
#ifndef _SERVO_H_
#define _SERVO_H_

#define servo_speed 20

//初始化6路PWM
void pwm_start(void);
//输入舵机角度变换为Compare值
void Translate_Angle2Pulse(double angle_1, double angle_2, double angle_3, double angle_4, double angle_5, double angle_6);
//根据传入世界坐标进行角度解算
void servo_angle_calculate(float target_x, float target_y, float target_z);
//实际角度设置
void servo_out(double angle_1, double angle_2, double angle_3, double angle_4, double angle_5,  double angle_6);
//（面向用户）控制舵机平缓运动到设定角度
void servo_control(double temp_target_angle_1,double temp_target_angle_2,double temp_target_angle_3,double temp_target_angle_4,double temp_target_angle_5,double temp_target_angle_6);
//机械臂复位初始状态函数
void servo_reset_begin(void);
//机械臂回正函数
void servo_reset(void);

#endif
