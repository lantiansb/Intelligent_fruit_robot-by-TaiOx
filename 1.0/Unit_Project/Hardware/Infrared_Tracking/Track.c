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

extern u8 Car_flag;//0-AGV小车停摆，1-小车到达抓取地点，2-小车到达打包点
extern u8 Arm_flag;//0-机械臂停摆，1-机械臂运行
u8 turn_flag = 1;
u16 turn_flag_flag;

/* Global functions ----------------------------------------------------------------------------------------*/
void Track_Init()
{
    //开启PA时钟
    CKCU_PeripClockConfig_TypeDef CKCUClock = {{ 0 }};
    CKCUClock.Bit.PA       = 1;
    CKCUClock.Bit.PC       = 1;
    CKCUClock.Bit.AFIO     = 1;
    CKCU_PeripClockConfig(CKCUClock, ENABLE);
    //控制循迹模块引脚为上拉
    AFIO_GPxConfig(GPIO_PA, AFIO_PIN_6|AFIO_PIN_7|AFIO_PIN_8|AFIO_PIN_10, AFIO_FUN_GPIO);
    /* Configure GPIO direction of input pins      输入模式                                                           */
    GPIO_DirectionConfig(HT_GPIOA, AFIO_PIN_6|AFIO_PIN_7|AFIO_PIN_8|AFIO_PIN_10, GPIO_DIR_IN);
    /* Configure GPIO pull resistor of input pins  上拉输入                                                           */
    GPIO_PullResistorConfig(HT_GPIOA, AFIO_PIN_6|AFIO_PIN_7|AFIO_PIN_8|AFIO_PIN_10, GPIO_PR_UP);
    /* 开启读取引脚状态                                                           */
    GPIO_InputConfig(HT_GPIOA, AFIO_PIN_6|AFIO_PIN_7|AFIO_PIN_8|AFIO_PIN_10, ENABLE);
    
    //电机控制脚,PC12、PC13置位，PC14、PC15低电平
    GPIO_DirectionConfig(HT_GPIOC, GPIO_PIN_12|GPIO_PIN_13, GPIO_DIR_OUT);
    GPIO_PullResistorConfig(HT_GPIOC, GPIO_PIN_12|GPIO_PIN_13, GPIO_PR_UP);
    GPIO_SetOutBits(HT_GPIOC, GPIO_PIN_12|GPIO_PIN_13);
    //初始化MCTM
    MCTM_PWM_Init();
    //开启MCTM
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
  * @brief  小车控制
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
        if( LineL2 == PRESS && LineR2 == NOPRESS && LineR1 == NOPRESS) //左最外检测
        {  
            turn_left();
            delay_ms(100);
        }
        else if ( LineR2 == PRESS && LineL2 == NOPRESS && LineL1 == NOPRESS) //右最外检测
        {  
            turn_right();
            delay_ms(100);
        }
        else if (LineL2 == NOPRESS && LineR2 == NOPRESS && LineL1 == PRESS && LineR1 == NOPRESS) //左内检测
        {   
            turn_left();
            delay_ms(10);
        }
        else if (LineL2 == NOPRESS && LineR2 == NOPRESS && LineL1 == NOPRESS && LineR1 == PRESS) //右内检测
        {   
            turn_right();
            delay_ms(10);
        }
        else if(LineL1 == PRESS && LineR1 == PRESS &&LineL2 == NOPRESS && LineR2 == NOPRESS) // 前进
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

