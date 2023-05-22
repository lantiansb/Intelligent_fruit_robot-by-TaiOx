#ifndef __PWM_H
#define __PWM_H

#include "ht32.h"
/* Exported functions --------------------------------------------------------------------------------------*/
void PWM_Init(void);
void MCTM_PWM_Init(void);
void PWM_SetFreq(HT_TM_TypeDef *TMx, u16 Reload);
void PWM_SetDuty(HT_TM_TypeDef* TMx, TM_CH_Enum TM_CH_n, u16 Cmp);

#endif
