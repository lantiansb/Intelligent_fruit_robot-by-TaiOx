/*
 * @Author: lantiansb lantiansb@foxmail.com
 * @Date: 2023-03-15 15:41:08
 * @LastEditors: lantiansb lantiansb@foxmail.com
 * @LastEditTime: 2023-03-31 10:55:48
 */

#include "ht32.h"
#include "pwm.h"

/**
 * @brief 初始化PWM(TM0、TM1)
 * 
 */
void PWM_Init(void)
{
    { /* Enable peripheral clock                                                                              */
        CKCU_PeripClockConfig_TypeDef CKCUClock = {{ 0 }};
        CKCUClock.Bit.GPTM0 = 1;
        CKCUClock.Bit.GPTM1 = 1;
        CKCUClock.Bit.AFIO = 1;     //使能AFIO时钟
        CKCU_PeripClockConfig(CKCUClock, ENABLE);
    }

    {/* Configure AFIO mode as TM function                                                                     */
        AFIO_GPxConfig(GPIO_PA, AFIO_PIN_0, AFIO_FUN_MCTM_GPTM);    //GT1.CH0
        AFIO_GPxConfig(GPIO_PA, AFIO_PIN_1, AFIO_FUN_MCTM_GPTM);    //GT1.CH1
        AFIO_GPxConfig(GPIO_PA, AFIO_PIN_2, AFIO_FUN_MCTM_GPTM);    //GT1.CH2
        AFIO_GPxConfig(GPIO_PA, AFIO_PIN_3, AFIO_FUN_MCTM_GPTM);    //GT1.CH3
        AFIO_GPxConfig(GPIO_PA, AFIO_PIN_4, AFIO_FUN_MCTM_GPTM);    //GT0.CH0
        AFIO_GPxConfig(GPIO_PA, AFIO_PIN_5, AFIO_FUN_MCTM_GPTM);    //GT0.CH1
    }

    {/* Configure PWM setting(period)                                                                     */
        TM_TimeBaseInitTypeDef TimeBaseInit;
        TM_TimeBaseStructInit(&TimeBaseInit);
        TimeBaseInit.Prescaler = 100 - 1;                          // Timer clock = CK_AHB / HTCFG_PWM_TM_PRESCALER(PSC)
        TimeBaseInit.CounterReload = (48000000/100/50) - 1;			//设置成50，再分频1000，得到50Hz，即20ms周期(ARR)
        TimeBaseInit.RepetitionCounter = 0;
        TimeBaseInit.CounterMode = TM_CNT_MODE_UP;
        TimeBaseInit.PSCReloadTime = TM_PSC_RLD_IMMEDIATE;
        TM_TimeBaseInit(HT_GPTM0, &TimeBaseInit);
        TM_CRRPreloadCmd(HT_GPTM0, ENABLE);
        /* Clear Update Event Interrupt flag since the "TM_TimeBaseInit()" writes the UEV1G bit                 */
        TM_ClearFlag(HT_GPTM0, TM_FLAG_UEV);
            
        TM_TimeBaseInit(HT_GPTM1, &TimeBaseInit);
        TM_CRRPreloadCmd(HT_GPTM1, ENABLE);
        /* Clear Update Event Interrupt flag since the "TM_TimeBaseInit()" writes the UEV1G bit                 */
        TM_ClearFlag(HT_GPTM1, TM_FLAG_UEV);
    }

    { /* Channel n output configuration                                                                       */

        TM_OutputInitTypeDef OutInit;

        OutInit.OutputMode = TM_OM_PWM1;
        OutInit.Control = TM_CHCTL_ENABLE;
        OutInit.ControlN = TM_CHCTL_DISABLE;
        OutInit.Polarity = TM_CHP_NONINVERTED;
        OutInit.PolarityN = TM_CHP_NONINVERTED;
        OutInit.IdleState = MCTM_OIS_LOW;
        OutInit.IdleStateN = MCTM_OIS_HIGH;
        OutInit.Compare = 0;
        OutInit.AsymmetricCompare = 0;
        OutInit.Channel = TM_CH_0;
        TM_OutputInit(HT_GPTM0, &OutInit); // CH0 Start Output as default value
        TM_CHCCRPreloadConfig(HT_GPTM0, TM_CH_0, ENABLE);
        
        OutInit.Channel = TM_CH_1;
        TM_OutputInit(HT_GPTM0, &OutInit); // CH1 Start Output as default value
        TM_CHCCRPreloadConfig(HT_GPTM0, TM_CH_1, ENABLE);

        OutInit.Channel = TM_CH_0;
        TM_OutputInit(HT_GPTM1, &OutInit); // CH0 Start Output as default value
        TM_CHCCRPreloadConfig(HT_GPTM1, TM_CH_0, ENABLE);

        OutInit.Channel = TM_CH_1;
        TM_OutputInit(HT_GPTM1, &OutInit); // CH1 Start Output as default value
        TM_CHCCRPreloadConfig(HT_GPTM1, TM_CH_1, ENABLE);
        
        OutInit.Channel = TM_CH_2;
        TM_OutputInit(HT_GPTM1, &OutInit); // CH2 Start Output as default value
        TM_CHCCRPreloadConfig(HT_GPTM1, TM_CH_2, ENABLE);

        OutInit.Channel = TM_CH_3;
        TM_OutputInit(HT_GPTM1, &OutInit); // CH3 Start Output as default value
        TM_CHCCRPreloadConfig(HT_GPTM1, TM_CH_3, ENABLE);
    }

}

void PWM_SetFreq(HT_TM_TypeDef *TMx, u16 Reload)
{
    TM_SetCounterReload(TMx, Reload);
}

void PWM_SetDuty(HT_TM_TypeDef* TMx, TM_CH_Enum TM_CH_n, u16 Cmp)
{
    TM_SetCaptureCompare(TMx, TM_CH_n, Cmp);
}
