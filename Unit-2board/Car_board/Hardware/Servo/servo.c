#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "math.h"
#include "servo.h"
#include "PWM.h"
#include "delay.h"

#define RAD2ANG (3.1415926535898/180.0)
#define ANG2RAD(N) ( (N) * (180.0/3.1415926535898) )

double pulse_1;
double pulse_2;
double pulse_3;
double pulse_4;
double pulse_5;
double pulse_6;

double angle_1;
double angle_2;
double angle_3;
double angle_4;
double angle_5;
double angle_6;

double now_angle_1 = 0;
double now_angle_2 = 0;
double now_angle_3 = 0;
double now_angle_4 = 0;
double now_angle_5 = 0;
double now_angle_6 = 0;

double target_angle_1;
double target_angle_2;
double target_angle_3;
double target_angle_4;
double target_angle_5;
double target_angle_6;

uint8_t center_x ,center_y ,color_type ;

double center_x_cm,center_y_cm;

double a1;
double a2;
double a3;
double a4;
double a5;
double a6;
    
double abs_angle_error_1;
double abs_angle_error_2;
double abs_angle_error_3;
double abs_angle_error_4;
double abs_angle_error_5;
double abs_angle_error_6;


/*************************** 调试变量，release时可删 *************************/
#define REV_OK		0	//接收完成标志
#define REV_WAIT	1	//接收未完成标志
unsigned char Servo_buf[20];
unsigned short loc_cnt = 0, loc_cntPre = 0;
/*****************************************************************************/

/**
 * @brief 开启PWM输出
 * 
 */
void pwm_start(void)
{
    PWM_Init();
    PWM_SetDuty(HT_GPTM0, TM_CH_0, 48000000/100/50*1.5/20);
    PWM_SetDuty(HT_GPTM0, TM_CH_1, 48000000/100/50*1.5/20);
    PWM_SetDuty(HT_GPTM1, TM_CH_0, 48000000/100/50*1.5/20);
    PWM_SetDuty(HT_GPTM1, TM_CH_1, 48000000/100/50*1.5/20);
    PWM_SetDuty(HT_GPTM1, TM_CH_2, 48000000/100/50*1.5/20);
    PWM_SetDuty(HT_GPTM1, TM_CH_3, 48000000/100/50*1.5/20);
    TM_Cmd(HT_GPTM0, ENABLE);
    TM_Cmd(HT_GPTM1, ENABLE);
}

/**
 * @brief 将角度转换为脉宽，进而改变PWM输出占空比
 * 
 * @param angle_1 机械臂舵机1角度
 * @param angle_2 机械臂舵机2角度
 * @param angle_3 机械臂舵机3角度
 * @param angle_4 机械臂舵机4角度
 * @param angle_5 机械臂舵机5角度
 * @param angle_6 机械臂舵机6角度
 */
void Translate_Angle2Pulse(double angle_1, double angle_2, double angle_3, double angle_4, double angle_5, double angle_6)
{
    pulse_1 = ((((angle_1 + 135) / 135) + 0.5) / 20) * (48000000/100/50);//注意TBSK20数字舵机转角为正负135°
    pulse_2 = ((((angle_2 + 135) / 135) + 0.5) / 20) * (48000000/100/50);
    
    pulse_3 = ((((angle_3 + 90) / 90) + 0.5) / 20) * (48000000/100/50);//注意MG996R转角为正负90°
    pulse_4 = ((((angle_4 + 135) / 135) + 0.5) / 20) * (48000000/100/50);    //8.8613°为爪子位置修正角
    pulse_5 = ((((angle_5 + 90) / 90) + 0.5) / 20) * (48000000/100/50);
    pulse_6 = ((((angle_6 + 90) / 90) + 0.5) / 20) * (48000000/100/50);
}

/**
 * @brief 面向用户，将角度传入生成对应的PWM
 * 
 * @param angle_1 机械臂舵机1角度
 * @param angle_2 机械臂舵机2角度
 * @param angle_3 机械臂舵机3角度
 * @param angle_4 机械臂舵机4角度
 * @param angle_5 机械臂舵机5角度
 * @param angle_6 机械臂舵机6角度
 */
void servo_out(double angle_1, double angle_2, double angle_3, double angle_4, double angle_5,  double angle_6)
{
	Translate_Angle2Pulse(angle_1, angle_2, angle_3, angle_4, angle_5, angle_6);
	if(pulse_1 != NULL)
	{
        PWM_SetDuty(HT_GPTM1, TM_CH_0, pulse_1);
	}
	if(pulse_2 != NULL)
	{
        PWM_SetDuty(HT_GPTM1, TM_CH_1, pulse_2);
	}
	if(pulse_3 != NULL)
	{
        PWM_SetDuty(HT_GPTM1, TM_CH_2, pulse_3);
	}
	if(pulse_4 != NULL)
	{
        PWM_SetDuty(HT_GPTM1, TM_CH_3, pulse_4);
	}
	if(pulse_5 != NULL)
	{
        PWM_SetDuty(HT_GPTM0, TM_CH_0, pulse_5);
	}
	if(pulse_6 != NULL)
	{
        PWM_SetDuty(HT_GPTM0, TM_CH_1, pulse_6);
	}
}

/**
 * @brief 根据目标位置，计算各舵机角度
 * 
 * @param target_x 目标在世界坐标中的x值
 * @param target_y 目标在世界坐标中的y值
 * @param target_z 目标在世界坐标中的z值
 */
void servo_angle_calculate(float target_x, float target_y, float target_z)
{
    double best_j1, best_j2, best_j3, best_x, best_y, best_z, err = 100;
	double a, b; //临时变量
	double L1 = 8.2952, L2 = 7.6508, L3 = 15.3392;//3节手臂的长度
	double m, n, t, q, p;//临时变量
	double j1, j2, j3, j0;//4个舵机的旋转角度
	double x1, y1, z1;//逆解后正解算出来的值，看是否与逆解值相等
	char i = 0;
	j0 = atan2(target_y, target_x);
	a = target_x / cos(j0);
	if (target_x == 0) a = target_y; //如果target_x为0，需要交换target_x，target_y
	b = target_z;

	for (j1 = -90; j1 < 90; j1++)
	{
		j1 *= RAD2ANG;
		j3 = acos((pow(a, 2) + pow(b, 2) + pow(L1, 2) - pow(L2, 2) - pow(L3, 2) - 2 * a * L1 * sin(j1) - 2 * b * L1 * cos(j1)) / (2 * L2 * L3));
		//if (abs(ANG2RAD(j3)) >= 135) { j1 = ANG2RAD(j1); continue; }
		m = L2 * sin(j1) + L3 * sin(j1) * cos(j3) + L3 * cos(j1) * sin(j3);
		n = L2 * cos(j1) + L3 * cos(j1) * cos(j3) - L3 * sin(j1) * sin(j3);
		t = a - L1 * sin(j1);
		p = pow(pow(n, 2) + pow(m, 2), 0.5);
		q = asin(m / p);
		j2 = asin(t / p) - q;
		//if (abs(ANG2RAD(j2)) >= 135) { j1 = ANG2RAD(j1); continue; }
		/***************计算正解然后与目标解对比，看解是否正确**************/
		x1 = (L1 * sin(j1) + L2 * sin(j1 + j2) + L3 * sin(j1 + j2 + j3)) * cos(j0);
		y1 = (L1 * sin(j1) + L2 * sin(j1 + j2) + L3 * sin(j1 + j2 + j3)) * sin(j0);
		z1 = L1 * cos(j1) + L2 * cos(j1 + j2) + L3 * cos(j1 + j2 + j3);
		j1 = ANG2RAD(j1);
		j2 = ANG2RAD(j2);
		j3 = ANG2RAD(j3);
		if (x1<(target_x + 1) && x1 >(target_x - 1) && y1<(target_y + 1) && y1 >(target_y - 1) && z1<(target_z + 1) && z1 >(target_z - 1) && (j1 + j2 + j3) >= 90)
		{
            if(j1<135 && j1>-135 && j2<90 && j2>-90 && j3<90 && j3>-90)
            {
//			    printf("j0:%f,j1:%f,j2:%f,j3:%f,target_x:%f,target_y:%f,target_z:%f,alpha:%f\r\n", ANG2RAD(j0), j1, j2, j3, x1, y1, z1, (j1 + j2 + j3));
                i = 1;
                if((x1-target_x)*(x1-target_x) + (y1-target_y)*(y1-target_y) + (z1-target_z)*(z1-target_z) < err)
                {
                    err = (x1-target_x)*(x1-target_x) + (y1-target_y)*(y1-target_y) + (z1-target_z)*(z1-target_z);
                    best_x = x1;
                    best_y = y1;
                    best_z = z1;
                    best_j1 = j1;
                    best_j2 = j2;
                    best_j3 = j3;
                }
            }
        }
	}
//	printf("\r\n");
	for (j1 = -90; j1 < 90; j1++)//这个循环是为了求解另一组解，j2 = asin(t / p) - q;j2 = -(asin(t / p) - q);多了个负号
	{
		j1 *= RAD2ANG;
		j3 = acos((pow(a, 2) + pow(b, 2) + pow(L1, 2) - pow(L2, 2) - pow(L3, 2) - 2 * a * L1 * sin(j1) - 2 * b * L1 * cos(j1)) / (2 * L2 * L3));
		//if (abs(ANG2RAD(j3)) >= 135) { j1 = ANG2RAD(j1); continue; }
		m = L2 * sin(j1) + L3 * sin(j1) * cos(j3) + L3 * cos(j1) * sin(j3);
		n = L2 * cos(j1) + L3 * cos(j1) * cos(j3) - L3 * sin(j1) * sin(j3);
		t = a - L1 * sin(j1);
		p = pow(pow(n, 2) + pow(m, 2), 0.5); 
		q = asin(m / p);
		j2 = -(asin(t / p) - q);
		//if (abs(ANG2RAD(j2)) >= 135) { j1 = ANG2RAD(j1); continue; }
		x1 = (L1 * sin(j1) + L2 * sin(j1 + j2) + L3 * sin(j1 + j2 + j3)) * cos(j0);
		y1 = (L1 * sin(j1) + L2 * sin(j1 + j2) + L3 * sin(j1 + j2 + j3)) * sin(j0);
		z1 = L1 * cos(j1) + L2 * cos(j1 + j2) + L3 * cos(j1 + j2 + j3);
		j1 = ANG2RAD(j1);
		j2 = ANG2RAD(j2);
		j3 = ANG2RAD(j3);
		if (x1<(target_x + 1) && x1 >(target_x - 1) && y1<(target_y + 1) && y1 >(target_y - 1) && z1<(target_z + 1) && z1 >(target_z - 1) && (j1 + j2 + j3) >= 90)
		{
            if(j1<135 && j1>-135 && j2<90 && j2>-90 && j3<90 && j3>-90)
            {
//			    printf("j0:%f,j1:%f,j2:%f,j3:%f,target_x:%f,target_y:%f,target_z:%f,alpha:%f\r\n", ANG2RAD(j0), j1, j2, j3, x1, y1, z1, (j1 + j2 + j3));
                i = 1;
                if((x1-target_x)*(x1-target_x) + (y1-target_y)*(y1-target_y) + (z1-target_z)*(z1-target_z) < err)
                {
                    err = (x1-target_x)*(x1-target_x) + (y1-target_y)*(y1-target_y) + (z1-target_z)*(z1-target_z);
                    best_x = x1;
                    best_y = y1;
                    best_z = z1;
                    best_j1 = j1;
                    best_j2 = j2;
                    best_j3 = j3;
                }
            }
		}
	}

	if (i == 0)printf("无解");
    
//	printf("\r\n");
    //计算结束，赋值目标角度
    j0 = ANG2RAD(j0);
    if(j0 < 135)
    {
        target_angle_1 = j0;
        target_angle_2 = best_j1;
        target_angle_3 = best_j2;
        target_angle_4 = best_j3;
    }
    else if(j0 >= 135)
    {
        target_angle_1 = j0 - 180;
        target_angle_2 = -best_j1;
        target_angle_3 = -best_j2;
        target_angle_4 = -best_j3;
    }
//    printf("j0:%f,j1:%f,j2:%f,j3:%f,target_x:%f,target_y:%f,target_z:%f,alpha:%f,err:%lf\r\n", ANG2RAD(j0), best_j1, best_j2, best_j3, best_x, best_y, best_z, (best_j1 + best_j2 + best_j3 + 12), err);
}

/**
 * @brief 控制舵机步进至目标角度，20ms（速度可调）每1°
 * 
 * @param temp_target_angle_1 目标angle1
 * @param temp_target_angle_2 目标angle2
 * @param temp_target_angle_3 目标angle3
 * @param temp_target_angle_4 目标angle4
 * @param temp_target_angle_5 目标angle5
 * @param temp_target_angle_6 目标angle6
 */
void servo_control(double temp_target_angle_1,double temp_target_angle_2,double temp_target_angle_3,double temp_target_angle_4,double temp_target_angle_5,double temp_target_angle_6)
{

    abs_angle_error_1 = fabs(temp_target_angle_1 - now_angle_1);   //误差绝对值
    abs_angle_error_2 = fabs(temp_target_angle_2 - now_angle_2);   //误差绝对值
    abs_angle_error_3 = fabs(temp_target_angle_3 - now_angle_3);   //误差绝对值
    abs_angle_error_4 = fabs(temp_target_angle_4 - now_angle_4);   //误差绝对值
    abs_angle_error_5 = fabs(temp_target_angle_5 - now_angle_5);   //误差绝对值
    abs_angle_error_6 = fabs(temp_target_angle_6 - now_angle_6);   //误差绝对值

    a1 = abs_angle_error_1;
    a2 = abs_angle_error_2;
    a3 = abs_angle_error_3;
    a4 = abs_angle_error_4;
    a5 = abs_angle_error_5;
    a6 = abs_angle_error_6;  

	if(temp_target_angle_1 <= 135 && temp_target_angle_1 >= -135)
	{
		for(;abs_angle_error_1 >= 3;abs_angle_error_1 --)
        {

            double pwm_angle_1 = (temp_target_angle_1 > now_angle_1 ? temp_target_angle_1 - abs_angle_error_1 : temp_target_angle_1 + abs_angle_error_1);

            servo_out(pwm_angle_1,now_angle_2,now_angle_3,now_angle_4,now_angle_5,now_angle_6);
            delay_ms(servo_speed);
            now_angle_1 = pwm_angle_1;
        }
        now_angle_1 = temp_target_angle_1;
        servo_out(temp_target_angle_1, now_angle_2, now_angle_3, now_angle_4, now_angle_5, now_angle_6);
	}
    
	if(temp_target_angle_2 <= 135 && temp_target_angle_2 >= -135)
	{
		for(;abs_angle_error_2 >= 3;abs_angle_error_2 --)
        {

            double pwm_angle_2 = (temp_target_angle_2 > now_angle_2 ? temp_target_angle_2 - abs_angle_error_2 : temp_target_angle_2 + abs_angle_error_2);

            servo_out(temp_target_angle_1,pwm_angle_2,now_angle_3,now_angle_4,now_angle_5,now_angle_6);
            delay_ms(servo_speed);
            now_angle_2 = pwm_angle_2;
        }
        now_angle_2 = temp_target_angle_2;
        servo_out(temp_target_angle_1, temp_target_angle_2, now_angle_3, now_angle_4, now_angle_5, now_angle_6);
	}

	if(temp_target_angle_3 <= 135 && temp_target_angle_3 >= -135)
	{
		for(;abs_angle_error_3 >= 3;abs_angle_error_3 --)
        {

            double pwm_angle_3 = (temp_target_angle_3 > now_angle_3 ? temp_target_angle_3 - abs_angle_error_3 : temp_target_angle_3 + abs_angle_error_3);

            servo_out(temp_target_angle_1,temp_target_angle_2,pwm_angle_3,now_angle_4,now_angle_5,now_angle_6);
            delay_ms(servo_speed);
            now_angle_3 = pwm_angle_3;
        }
        now_angle_3 = temp_target_angle_3;
        servo_out(temp_target_angle_1, temp_target_angle_2, temp_target_angle_3, now_angle_4, now_angle_5, now_angle_6);
	}

	if(temp_target_angle_4 <= 135 && temp_target_angle_4 >= -135)
	{
		for(;abs_angle_error_4 >= 3;abs_angle_error_4 --)
        {

            double pwm_angle_4 = (temp_target_angle_4 > now_angle_4 ? temp_target_angle_4 - abs_angle_error_4 : temp_target_angle_4 + abs_angle_error_4);
            servo_out(temp_target_angle_1,temp_target_angle_2,temp_target_angle_3,pwm_angle_4,now_angle_5,now_angle_6);
            delay_ms(servo_speed);
            now_angle_4 = pwm_angle_4;
        }
        now_angle_4 = temp_target_angle_4;
        servo_out(temp_target_angle_1,temp_target_angle_2,temp_target_angle_3,temp_target_angle_4,now_angle_5,now_angle_6);
	}
    
	if(temp_target_angle_5 <= 135 && temp_target_angle_5 >= -135)
	{
		for(;abs_angle_error_5 >= 3;abs_angle_error_5 --)
        {

            double pwm_angle_5 = (temp_target_angle_5 > now_angle_5 ? temp_target_angle_5 - abs_angle_error_5 : temp_target_angle_5 + abs_angle_error_5);
            servo_out(temp_target_angle_1,temp_target_angle_2,temp_target_angle_3,temp_target_angle_4,pwm_angle_5,now_angle_6);
            delay_ms(servo_speed);
            now_angle_5 = pwm_angle_5;
        }
        now_angle_5 = temp_target_angle_5;
        servo_out(temp_target_angle_1,temp_target_angle_2,temp_target_angle_3,temp_target_angle_4,temp_target_angle_5,now_angle_6);
	}

    //爪子无需做减速处理，防止抓力不足
	if(temp_target_angle_6 <= 135 && temp_target_angle_6 >= -135)
	{
		for(;abs_angle_error_6 >= 3;abs_angle_error_6 --)
        {

            double pwm_angle_6 = (temp_target_angle_6 > now_angle_6 ? temp_target_angle_6 - abs_angle_error_6 : temp_target_angle_6 + abs_angle_error_6);
            servo_out(temp_target_angle_1,temp_target_angle_2,temp_target_angle_3,temp_target_angle_4,temp_target_angle_5,pwm_angle_6);
            delay_ms(5);
            now_angle_6 = pwm_angle_6;
        }
        now_angle_6 = temp_target_angle_6;
        servo_out(temp_target_angle_1,temp_target_angle_2,temp_target_angle_3,temp_target_angle_4,temp_target_angle_5,temp_target_angle_6);
	}

    abs_angle_error_1  = a1;
    abs_angle_error_2  = a2;
    abs_angle_error_3  = a3;
    abs_angle_error_4  = a4;
    abs_angle_error_5  = a5;
    abs_angle_error_6  = a6;
}

/**
 * @brief 机械臂复位初始状态函数，机械臂若卷曲静止时进行重启，需要手动帮助直立，防止舵机转矩不够而过电流重启
 * 
 */
void servo_reset_begin(void)
{
    servo_out(0,0,0,0,0,0);
    delay_ms(1000);
    now_angle_1 = 0;
    now_angle_2 = 0;
    now_angle_3 = 0;
    now_angle_4 = 0;
    now_angle_5 = 0;
    now_angle_6 = 0;
    delay_ms(1000);
}

/**
 * @brief 回正函数，至于为什么不用servo_control()是因为6个舵机都调速太慢了
 * 
 */
void servo_reset(void)
{
    double i1 = 0,i2 = 0,i3 = 0,i4 = 0;
    now_angle_6 = 0;
    double b5 = now_angle_5;
    double b4 = now_angle_4;
    double b3 = now_angle_3;
    double b2 = now_angle_2;
    for(i1 = 10 * b5;i1 >= 0;i1 -= b5)
    {
        servo_out(0,now_angle_2,now_angle_3,now_angle_4,0.1 * i1,0);
        delay_ms(10);
        now_angle_5 = 0.1 *i1;

    }
    servo_out(0,now_angle_2,now_angle_3,now_angle_4,0,0);

    for(i2 = 10 * b4;i2 >= 0;i2 -= b4)
    {
        servo_out(0,now_angle_2,now_angle_3,0.1 * i2,0,0);
        delay_ms(30);
        now_angle_4 = 0.1 * i2;

    }
    servo_out(0,now_angle_2,now_angle_3,0,0,0);

    for(i4 = 10 * b3;i4 >= 0;i4 -= b3)
    {
        servo_out(0,now_angle_2,0.1 * i4,0,0,0);
        delay_ms(30);
        now_angle_3 = 0.1 * i4;
    }    
    servo_out(0,now_angle_2,0,0,0,0);

    for(i3 = 10 * b2;i3 >= 0;i3 -= b2)
    {
        servo_out(0,0.1 * i3,0,0,0,0);
        delay_ms(50);
        now_angle_2 = 0.1 * i3;
    }    
    servo_out(0,0,0,0,0,0);
    
    now_angle_1 = 0;   
}

/**
 * @brief 进行一次坐标夹取并复位
 * 
 */
void servo_catch(void)
{
    servo_control(0,0,0,0,0,0);
    servo_control(target_angle_1,target_angle_2,target_angle_3,target_angle_4,0,0);
    delay_ms(1000);
    servo_control(target_angle_1,target_angle_2,target_angle_3,target_angle_4,0,35);
    delay_ms(1000);
    servo_control(0,0,0,0,0,35);
    servo_control(0,-20,-45,-25,0,0);
    delay_ms(2000);
    servo_control(0,0,0,134,0,0);
    servo_control(0,0,0,0,0,0);
}


/*************************** 以下均为调试函数，可删！ *************************/
_Bool Location_WaitRecive(void)//获取串口接收状态
{

	if(loc_cnt == 0) 							//如果接收计数为0 则说明没有处于接收数据中，所以直接跳出，结束函数
		return REV_WAIT;
		
	if(loc_cnt == loc_cntPre)				//如果上一次的值和这次相同，则说明接收完毕
	{
		loc_cnt = 0;							//清0接收计数
			
		return REV_OK;								//返回接收完成标志
	}
		
	loc_cntPre = loc_cnt;					//置为相同
	
	return REV_WAIT;								//返回接收未完成标志

}

//实时获取串口给定抓取坐标
float *GetLocation(unsigned short timeOut)
{
    float location_xyz[3];//返回对应世界坐标的xyz值
	char ptrIPD[10];
    char *token, *next_token;
    const char delimiters[] = ","; // 定义分隔符
	strcpy(ptrIPD, (const char*)Servo_buf);
	do
	{
        token = strtok_r(ptrIPD, delimiters, &next_token);
        if (token != NULL) 
        {
            location_xyz[0] = atof(token);//对x赋值
            token = strtok_r(NULL, delimiters, &next_token);
            if (token != NULL) 
            {
                location_xyz[1] = atof(token);
                memset(Servo_buf, 0, sizeof(Servo_buf));
                loc_cnt = 0;
                printf("x: %.2f, y: %.2f\n", location_xyz[0], location_xyz[1]);
            }
            location_xyz[2] = -2;
            return location_xyz;
        }
		delay_ms(5);													//延时等待
	} while(timeOut--);
	
	return NULL;														//超时还未找到，返回空指针

}

//串口1中断，用于调试机械臂
void USART1_IRQHandler(void)
{
	if(USART_GetFlagStatus(HT_USART1, USART_FLAG_RXDR) != RESET) //接收中断
	{
		if(loc_cnt >= sizeof(Servo_buf))	loc_cnt = 0; //防止串口被刷爆
		Servo_buf[loc_cnt++] = HT_USART1->DR;
		USART_ClearFlag(HT_USART1, USART_FLAG_RSADD);
	}
}