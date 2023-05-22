#include "MachineArm.h"
#include "ht32.h"
#include "servo.h"
#include "PWM.h"
#include "delay.h"
#include "K210.h"

float location_xyz[3];//��ǰץȡ��Ŀ�����������

//��servo.c�е������Ļ�е��λ�˲���
extern double target_angle_1;
extern double target_angle_2;
extern double target_angle_3;
extern double target_angle_4;

/**
 * @brief ����һ���Ӿ�ץȡ��
 * 
 * @param cmd-Ҫʶ���ˮ��
 *
 * @retravl 0-ʧ�ܣ�1-���
 */
_Bool Eye_Catch(char *cmd)
{
    servo_control(0,0,0,134,0,0);
    float *location_xyz_cpy = NULL;
    delay_ms(1000);
    K210_SendCmd(cmd);
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
    
    return 1;
}

