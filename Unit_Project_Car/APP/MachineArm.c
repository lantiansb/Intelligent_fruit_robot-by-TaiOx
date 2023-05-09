#include "MachineArm.h"
#include "ht32.h"
#include "servo.h"
#include "PWM.h"
#include "delay.h"
#include "K210.h"

float location_xyz[3];//当前抓取的目标的世界坐标

//从servo.c中调过来的机械臂位姿参数
extern double target_angle_1;
extern double target_angle_2;
extern double target_angle_3;
extern double target_angle_4;

/**
 * @brief 进行一次视觉抓取！
 * 
 * @param cmd-要识别的水果
 *
 * @retravl 0-失败，1-完成
 */
_Bool Eye_Catch(char *cmd)
{
    servo_control(0,0,0,134,0,0);
    float *location_xyz_cpy = NULL;
    delay_ms(1000);
    K210_SendCmd(cmd);
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
    
    return 1;
}

