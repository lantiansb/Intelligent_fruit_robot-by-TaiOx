#include "Track.h"
#include "pwm.h"
#include "delay.h"

#define NOPRESS		(1)
#define PRESS	(0)
#if ForBack
    u8 LineL2;
    u8 LineL1;
    u8 LineR1;
    u8 LineR2;
#else
    #define LineL2  GPIO_ReadInBit(HT_GPIOA, GPIO_PIN_9)
    #define LineL1  GPIO_ReadInBit(HT_GPIOA, GPIO_PIN_8)
    #define LineR1  GPIO_ReadInBit(HT_GPIOA, GPIO_PIN_7)
    #define LineR2  GPIO_ReadInBit(HT_GPIOA, GPIO_PIN_6)
#endif

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
    //����ѭ��ģ������Ϊ����
    AFIO_GPxConfig(GPIO_PA, AFIO_PIN_6|AFIO_PIN_7|AFIO_PIN_8|AFIO_PIN_10, AFIO_FUN_GPIO);
    /* Configure GPIO direction of input pins      ����ģʽ                                                           */
    GPIO_DirectionConfig(HT_GPIOA, AFIO_PIN_6|AFIO_PIN_7|AFIO_PIN_8|AFIO_PIN_10, GPIO_DIR_IN);
    /* Configure GPIO pull resistor of input pins  ��������                                                           */
    GPIO_PullResistorConfig(HT_GPIOA, AFIO_PIN_6|AFIO_PIN_7|AFIO_PIN_8|AFIO_PIN_10, GPIO_PR_UP);
    /* ������ȡ����״̬                                                           */
    GPIO_InputConfig(HT_GPIOA, AFIO_PIN_6|AFIO_PIN_7|AFIO_PIN_8|AFIO_PIN_10, ENABLE);
    
    //������ƽ�,PC12��PC13��λ��PC14��PC15�͵�ƽ
    GPIO_DirectionConfig(HT_GPIOC, GPIO_PIN_12|GPIO_PIN_13, GPIO_DIR_OUT);
    GPIO_PullResistorConfig(HT_GPIOC, GPIO_PIN_12|GPIO_PIN_13, GPIO_PR_UP);
    GPIO_SetOutBits(HT_GPIOC, GPIO_PIN_12|GPIO_PIN_13);
    //��ʼ��MCTM
    MCTM_PWM_Init();
    //����MCTM
    TM_Cmd(HT_MCTM0, ENABLE);
}

void Forward_Backward(unsigned char ES)
{
    if(ES)
    {
        GPIO_SetOutBits(HT_GPIOC, GPIO_PIN_12|GPIO_PIN_13);
        GPIO_ClearOutBits(HT_GPIOC, GPIO_PIN_14|GPIO_PIN_15);
    }
    else
    {
        GPIO_ClearOutBits(HT_GPIOC, GPIO_PIN_12|GPIO_PIN_13);
        GPIO_SetOutBits(HT_GPIOC, GPIO_PIN_14|GPIO_PIN_15);
    }
}

void Go()
{
    PWM_SetDuty(HT_MCTM0, TM_CH_2, 48000000/80000*58/100);
    PWM_SetDuty(HT_MCTM0, TM_CH_3, 48000000/80000*58/100);
}

void turn_left()
{
    if(ForBack)
    {
        PWM_SetDuty(HT_MCTM0, TM_CH_2, 48000000/80000*0/100);
        PWM_SetDuty(HT_MCTM0, TM_CH_3, 48000000/80000*80/100);
    }
    else
    {
        PWM_SetDuty(HT_MCTM0, TM_CH_2, 48000000/80000*70/100);
        PWM_SetDuty(HT_MCTM0, TM_CH_3, 48000000/80000*20/100);
    }
}

void turn_right()
{
    if(ForBack)
    {
        PWM_SetDuty(HT_MCTM0, TM_CH_2, 48000000/80000*80/100);
        PWM_SetDuty(HT_MCTM0, TM_CH_3, 48000000/80000*0/100);
    }
    else
    {
        PWM_SetDuty(HT_MCTM0, TM_CH_2, 48000000/80000*30/100);
        PWM_SetDuty(HT_MCTM0, TM_CH_3, 48000000/80000*70/100);
    }
}

void stop()
{
    PWM_SetDuty(HT_MCTM0, TM_CH_2, 48000000/80000*0/100);
    PWM_SetDuty(HT_MCTM0, TM_CH_3, 48000000/80000*0/100);
}

/*********************************************************************************************************//**
  * @brief  С������
  * @retval None
  ***********************************************************************************************************/
void Car_CTRL(void)
{
    LineL2 = GPIO_ReadInBit(HT_GPIOA, GPIO_PIN_6);
    LineL1 = GPIO_ReadInBit(HT_GPIOA, GPIO_PIN_7);
    LineR1 = GPIO_ReadInBit(HT_GPIOA, GPIO_PIN_8);
    LineR2 = GPIO_ReadInBit(HT_GPIOA, GPIO_PIN_10);
    while(1)
    {
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
                if(++turn_flag >= 5)
                {
                    turn_flag = 0;
                    if(++Car_flag == 1)
                        Arm_flag = 1;
                    break;
                }
            }
        }
    }
}

