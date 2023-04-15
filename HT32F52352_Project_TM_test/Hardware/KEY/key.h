#ifndef __KEY_H
#define __KEY_H	 
#include "sys.h"


#define KEY_GPIO_GROUP             (GPIO_PA)
#define KEY1_PIN                   (GPIO_PIN_0)
#define KEY2_PIN                   (GPIO_PIN_1)
#define KEY_AFIO_MODE              (AFIO_MODE_DEFAULT) //默认模式：AFIO_MODE_DEFAULT ，AFIO_MODE_1~15对应模式1~15

void KEY_Init(void);//初始化

		 				    
#endif
