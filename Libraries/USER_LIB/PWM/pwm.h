/*
 * @Author: lantiansb lantiansb@foxmail.com
 * @Date: 2023-03-15 15:41:08
 * @LastEditors: lantiansb lantiansb@foxmail.com
 * @LastEditTime: 2023-03-30 22:36:18
 */

#ifndef __PWM_H
#define __PWM_H

#include "ht32.h"

void PWM_Init(void);
void PWM_SetFreq(HT_TM_TypeDef *TMx, u16 Reload);
void PWM_SetDuty(HT_TM_TypeDef* TMx, TM_CH_Enum TM_CH_n, u16 Cmp);

#endif
