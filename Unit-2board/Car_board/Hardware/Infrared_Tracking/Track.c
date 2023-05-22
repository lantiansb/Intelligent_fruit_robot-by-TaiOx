#include "Track.h"
#include "pwm.h"
#include "delay.h"

#define NOPRESS		(1)
#define PRESS	(0)

u8 ForBack = 1;

//���ڴ洢��ǰС��λ��
u8 LineL2;
u8 LineL1;
u8 LineR1;
u8 LineR2;

extern u8 Car_flag;//0-AGVС��ͣ�ڣ�1-С������ץȡ�ص㣬2-С����������
extern u8 Arm_flag;//0-��е��ͣ�ڣ�1-��е������
u8 turn_flag = 1;
u16 turn_flag_flag;

/* Global functions ----------------------------------------------------------------------------------------*/
void Track_Init()
{
    //����PAʱ��
    CKCU_PeripClockConfig_TypeDef CKCUClock = {{ 0 }};
    CKCUClock.Bit.PA       = 1;
    CKCUClock.Bit.PC       = 1;
    CKCUClock.Bit.AFIO     = 1;
    CKCU_PeripClockConfig(CKCUClock, ENABLE);
    //��������ѭ��ģ������Ϊ����
    AFIO_GPxConfig(GPIO_PA, AFIO_PIN_6|AFIO_PIN_7|AFIO_PIN_8|AFIO_PIN_10, AFIO_FUN_GPIO);
    /* Configure GPIO direction of input pins      ����ģʽ                                                           */
    GPIO_DirectionConfig(HT_GPIOA, AFIO_PIN_6|AFIO_PIN_7|AFIO_PIN_8|AFIO_PIN_10, GPIO_DIR_IN);
    /* Configure GPIO pull resistor of input pins  ��������                                                           */
    GPIO_PullResistorConfig(HT_GPIOA, AFIO_PIN_6|AFIO_PIN_7|AFIO_PIN_8|AFIO_PIN_10, GPIO_PR_UP);
    /* ������ȡ����״̬                                                           */
    GPIO_InputConfig(HT_GPIOA, AFIO_PIN_6|AFIO_PIN_7|AFIO_PIN_8|AFIO_PIN_10, ENABLE);
    
    //���Ʒ���ѭ��ģ������Ϊ����
    AFIO_GPxConfig(GPIO_PA, AFIO_PIN_0|AFIO_PIN_1|AFIO_PIN_2|AFIO_PIN_3, AFIO_FUN_GPIO);
    /* Configure GPIO direction of input pins      ����ģʽ                                                           */
    GPIO_DirectionConfig(HT_GPIOA, AFIO_PIN_0|AFIO_PIN_1|AFIO_PIN_2|AFIO_PIN_3, GPIO_DIR_IN);
    /* Configure GPIO pull resistor of input pins  ��������                                                           */
    GPIO_PullResistorConfig(HT_GPIOA, AFIO_PIN_0|AFIO_PIN_1|AFIO_PIN_2|AFIO_PIN_3, GPIO_PR_UP);
    /* ������ȡ����״̬                                                           */
    GPIO_InputConfig(HT_GPIOA, AFIO_PIN_0|AFIO_PIN_1|AFIO_PIN_2|AFIO_PIN_3, ENABLE);
    
    //������ƽ�,PC12��PC13��λ��PC14��PC15�͵�ƽ��Ĭ��Ϊ����
    GPIO_DirectionConfig(HT_GPIOC, GPIO_PIN_12|GPIO_PIN_13, GPIO_DIR_OUT);
    GPIO_PullResistorConfig(HT_GPIOC, GPIO_PIN_12|GPIO_PIN_13, GPIO_PR_UP);
    GPIO_SetOutBits(HT_GPIOC, GPIO_PIN_12|GPIO_PIN_13);
    GPIO_ClearOutBits(HT_GPIOC, GPIO_PIN_14|GPIO_PIN_15);
    //��ʼ��MCTM
    MCTM_PWM_Init();
    //����MCTM
    TM_Cmd(HT_MCTM0, ENABLE);
}

void Go()
{
    PWM_SetDuty(HT_MCTM0, TM_CH_2, 48000000/80000*60/100);
    PWM_SetDuty(HT_MCTM0, TM_CH_3, 48000000/80000*60/100);
}

void turn_left()
{
    PWM_SetDuty(HT_MCTM0, TM_CH_2, 48000000/80000*0/100);
    PWM_SetDuty(HT_MCTM0, TM_CH_3, 48000000/80000*68/100);
}

void turn_right()
{
    PWM_SetDuty(HT_MCTM0, TM_CH_2, 48000000/80000*68/100);
    PWM_SetDuty(HT_MCTM0, TM_CH_3, 48000000/80000*0/100);
}

void stop()
{
    PWM_SetDuty(HT_MCTM0, TM_CH_2, 48000000/80000*0/100);
    PWM_SetDuty(HT_MCTM0, TM_CH_3, 48000000/80000*0/100);
}

/*********************************************************************************************************//**
  * @brief  С������ѭ��
  * @retval None
  ***********************************************************************************************************/
void Car_CTRL(void)
{
    GPIO_SetOutBits(HT_GPIOC, GPIO_PIN_12|GPIO_PIN_13);
    GPIO_ClearOutBits(HT_GPIOC, GPIO_PIN_14|GPIO_PIN_15);
    while(1)
    {
        //����ǰƫ����
        {
            LineL2 = GPIO_ReadInBit(HT_GPIOA, GPIO_PIN_6);
            LineL1 = GPIO_ReadInBit(HT_GPIOA, GPIO_PIN_7);
            LineR1 = GPIO_ReadInBit(HT_GPIOA, GPIO_PIN_8);
            LineR2 = GPIO_ReadInBit(HT_GPIOA, GPIO_PIN_10);
        }
        //ѭ��
        if( LineL2 == PRESS && LineR2 == NOPRESS && LineR1 == NOPRESS) //��������
        {  
            turn_left();
            delay_ms(100);
        }
        else if ( LineR2 == PRESS && LineL2 == NOPRESS && LineL1 == NOPRESS) //��������
        {  
            turn_right();
            delay_ms(100);
        }
        else if (LineL2 == NOPRESS && LineR2 == NOPRESS && LineL1 == PRESS && LineR1 == NOPRESS) //���ڼ��
        {   
            turn_left();
            delay_ms(10);
        }
        else if (LineL2 == NOPRESS && LineR2 == NOPRESS && LineL1 == NOPRESS && LineR1 == PRESS) //���ڼ��
        {   
            turn_right();
            delay_ms(10);
        }
        else if(LineL1 == PRESS && LineR1 == PRESS &&LineL2 == NOPRESS && LineR2 == NOPRESS) // ǰ��
        {  
            Go();
        }
        else if(LineL1 == PRESS && LineR1 == PRESS &&LineL2 == PRESS && LineR2 == PRESS)
        {
            stop();
            if(++turn_flag_flag >= 65535)
            {
                turn_flag_flag = 0;
                if(++turn_flag >= 2)
                {
                    turn_flag = 0;
                    ForBack = 0;
                    break;
                }
            }
        }
    }
}


/*********************************************************************************************************//**
  * @brief  С������ѭ��
  * @retval None
  ***********************************************************************************************************/
void Car_N_CTRL(void)
{
    GPIO_ClearOutBits(HT_GPIOC, GPIO_PIN_12|GPIO_PIN_13);
    GPIO_SetOutBits(HT_GPIOC, GPIO_PIN_14|GPIO_PIN_15);
    Go();
    delay_ms(1000);//������ʱ���ȵ��˳���ɫ���򣬽���ѭ������
    while(1)
    {
        //����ǰƫ����
        {
            LineL2 = GPIO_ReadInBit(HT_GPIOA, GPIO_PIN_3);
            LineL1 = GPIO_ReadInBit(HT_GPIOA, GPIO_PIN_2);
            LineR1 = GPIO_ReadInBit(HT_GPIOA, GPIO_PIN_1);
            LineR2 = GPIO_ReadInBit(HT_GPIOA, GPIO_PIN_0);
        }
        //ѭ��
        if( LineL2 == PRESS && LineR2 == NOPRESS && LineR1 == NOPRESS) //��������
        {  
            turn_left();
            delay_ms(100);
        }
        else if ( LineR2 == PRESS && LineL2 == NOPRESS && LineL1 == NOPRESS) //��������
        {  
            turn_right();
            delay_ms(100);
        }
        else if (LineL2 == NOPRESS && LineR2 == NOPRESS && LineL1 == PRESS && LineR1 == NOPRESS) //���ڼ��
        {   
            turn_left();
            delay_ms(10);
        }
        else if (LineL2 == NOPRESS && LineR2 == NOPRESS && LineL1 == NOPRESS && LineR1 == PRESS) //���ڼ��
        {   
            turn_right();
            delay_ms(10);
        }
        else if(LineL1 == PRESS && LineR1 == PRESS &&LineL2 == NOPRESS && LineR2 == NOPRESS) // ǰ��
        {  
            Go();
        }
        else if(LineL1 == PRESS && LineR1 == PRESS &&LineL2 == PRESS && LineR2 == PRESS)
        {
            stop();
            if(++turn_flag_flag >= 65535)
            {
                turn_flag_flag = 0;
                if(++turn_flag >= 2)
                {
                    turn_flag = 0;
                    ForBack = 1;
                    break;
                }
            }
        }
    }
}

