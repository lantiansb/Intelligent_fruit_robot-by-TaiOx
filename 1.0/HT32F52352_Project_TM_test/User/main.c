#include "ht32.h"
#include "sys.h"
#include "led.h"
#include "delay.h"
#include "key.h"
#include "usart.h"
#include "pwm.h"
#include "GPIOinput.h"
#include "delay.h"

int main(void)
{
//	PWM_Init();
	MCTM_PWM_Init();
//    PWM_SetFreq(PWM_FREQ_40K);					//Æô¶¯¶æ»ú£¬50Hz
//    PWM_UpdateDuty(TM_CH_0, PWM_FREQ_40K * 0.125);
//    TM_Cmd(HT_GPTM0, ENABLE);
//    TM_Cmd(HT_GPTM1, ENABLE);
    /* MCTM counter enable                                                                                    */
    TM_Cmd(HT_MCTM0, ENABLE);
  while (1);	
}
