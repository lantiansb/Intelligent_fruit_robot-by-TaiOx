#include <stdio.h>
#include "math.h"
#include "servo.h"
#include "PWM.h"
#include "delay.h"


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

double R;        //底座半径
double length_1; //底座高度
double length_2; //机械臂1长度
double length_3; //机械臂2长度
double length_4; //机械臂3长度

float pi = 3.141593;

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
    pulse_1 = ((((angle_1 + 90) / 90) + 0.5) / 20) * (48000000/100/50);
    pulse_2 = ((((angle_2 + 90) / 90) + 0.5) / 20) * (48000000/100/50);
    pulse_3 = ((((angle_3 + 90) / 90) + 0.5) / 20) * (48000000/100/50);
    pulse_4 = ((((angle_4 + 90) / 90) + 0.5) / 20) * (48000000/100/50);
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
 * @param target_y 目标在世界坐标中的x值
 * @param target_z 目标在世界坐标中的x值
 */
void servo_angle_calculate(float target_x, float target_y, float target_z)
{
    if (target_y >= 18)
        target_y = 18;
    else if(target_y <= 3)
        target_y = 3;
	float len_1, len_2, len_3, len_4;   //len_1为底座高度，其余三个为三机械臂长度
	float j1,j2,j3,j4 ;   //四个姿态角
	float L, H, bottom_r;					//	L =	a2*sin(j2) + a3*sin(j2 + j3);H = a2*cos(j2) + a3*cos(j2 + j3); R为底部圆盘半径
	float j_sum;			//j2,j3,j4之和
	float len, high;   //总长度，总高度
	float cos_j3, sin_j3; //用来储存cosj3，sinj3数值
	float cos_j2, sin_j2;
	float k1, k2;
	int i;
	float n, m;
	n = 0;
	m = 0;

	//输入初始值
	bottom_r = 0; 		//底部圆盘半径
	len_1 = 6.487; 	//底部圆盘高度
	//机械臂长度
	len_2 = 8.314;
	len_3 = 7.6;
	len_4 = 13.658;

	if (target_x == 0)
		j1 = 90;
	else
		j1 = 90 - atan(target_x / (target_y + bottom_r)) * (57.3);


	for (i = 0; i <= 180; i ++)
	{
		j_sum = 3.1415927 * i / 180;

		len = sqrt((target_y + bottom_r) * (target_y + bottom_r) + target_x * target_x);
		high = target_z;

		L = len - len_4 * sin(j_sum);
		H = high - len_4 * cos(j_sum) - len_1;

		cos_j3 = ((L * L) + (H * H) - ((len_2) * (len_2)) - ((len_3) * (len_3))) / (2 * (len_2) * (len_3));
		sin_j3 = (sqrt(1 - (cos_j3) * (cos_j3)));

		j3 = atan((sin_j3) / (cos_j3)) * (57.3);

		k2 = len_3 * sin(j3 / 57.3);
		k1 = len_2 + len_3 * cos(j3 / 57.3);

		cos_j2 = (k2 * L + k1 * H) / (k1 * k1 + k2 * k2);
		sin_j2 = (sqrt(1 - (cos_j2) * (cos_j2)));

		j2 = atan((sin_j2) / (cos_j2)) * 57.3;
		j4 = j_sum * 57.3 - j2 - j3;

		if (j2 >= 0 && j3 >= 0 && j4 >= -90 && j2 <= 180 && j3 <= 180 && j4 <= 90)
		{
			n ++;
		}
	}

	for (i = 0; i <= 180; i ++)
	{
		j_sum = 3.1415927 * i / 180;

		len = sqrt((target_y + bottom_r) * (target_y + bottom_r) + target_x * target_x);
		high = target_z;

		L = len - len_4 * sin(j_sum);
		H = high - len_4 * cos(j_sum) - len_1;

		cos_j3 = ((L * L) + (H * H) - ((len_2) * (len_2)) - ((len_3) * (len_3))) / (2 * (len_2) * (len_3));
		sin_j3 = (sqrt(1 - (cos_j3) * (cos_j3)));

		j3 = atan((sin_j3) / (cos_j3)) * (57.3);

		k2 = len_3 * sin(j3 / 57.3);
		k1 = len_2 + len_3 * cos(j3 / 57.3);

		cos_j2 = (k2 * L + k1 * H) / (k1 * k1 + k2 * k2);
		sin_j2 = (sqrt(1 - (cos_j2) * (cos_j2)));

		j2 = atan((sin_j2) / (cos_j2)) * 57.3;
		j4 = j_sum * 57.3 - j2 - j3;

		if (j2 >= 0 && j3 >= 0 && j4 >= -90 && j2 <= 180 && j3 <= 180 && j4 <= 90)
		{
			m ++;
			if (m == n / 2 || m == (n + 1) / 2)
				break;			
		}
	}
	target_angle_1 = j1;
	target_angle_2 = j2;
	target_angle_3 = j3;
	target_angle_4 = j4;
//	printf("center_x: %f\r\n center_y: %f\r\n taret_angle_1: %f\r\n taret_angle_2: %f\r\n taret_angle_3: %f\r\n taret_angle_4: %f\r\n",target_x,target_y,j1,j2,j3,j4);
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

	if(temp_target_angle_1 <= 90 && temp_target_angle_1 >= -90)
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
    
	if(temp_target_angle_2 <= 90 && temp_target_angle_2 >= -90)
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

	if(temp_target_angle_3 <= 90 && temp_target_angle_3 >= -90)
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

	if(temp_target_angle_4 <= 90 && temp_target_angle_4 >= -90)
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
    
	if(temp_target_angle_5 <= 90 && temp_target_angle_5 >= -90)
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
	if(temp_target_angle_6 <= 180 && temp_target_angle_6 >= -180)
	{
//		for(;abs_angle_error_6 >= 3;abs_angle_error_6 --)
//        {

//            double pwm_angle_6 = (temp_target_angle_6 > now_angle_6 ? temp_target_angle_6 - abs_angle_error_6 : temp_target_angle_6 + abs_angle_error_6);
//            servo_out(temp_target_angle_1,temp_target_angle_2,temp_target_angle_3,temp_target_angle_4,temp_target_angle_5,pwm_angle_6);
//            delay_ms(30);
//            now_angle_6 = pwm_angle_6;
//        }
//        now_angle_6 = temp_target_angle_6;
//        servo_out(temp_target_angle_1,temp_target_angle_2,temp_target_angle_3,temp_target_angle_4,temp_target_angle_5,temp_target_angle_6);
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
