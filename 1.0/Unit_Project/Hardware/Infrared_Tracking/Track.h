#ifndef __TRACK_H
#define __TRACK_H

#include "ht32.h"

#define ForBack 1

/* Exported functions --------------------------------------------------------------------------------------*/
void Track_Init(void);//循迹初始化函数
void Car_CTRL(void);//小车运动控制函数
void Go(void);
void turn_right(void);

#endif
