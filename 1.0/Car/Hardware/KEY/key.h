#ifndef __KEY_H
#define __KEY_H	 
#include "sys.h"


#define KEY_GPIO_GROUP             (GPIO_PA)
#define KEY1_PIN                   (GPIO_PIN_0)
#define KEY2_PIN                   (GPIO_PIN_1)
#define KEY_AFIO_MODE              (AFIO_MODE_DEFAULT) //Ĭ��ģʽ��AFIO_MODE_DEFAULT ��AFIO_MODE_1~15��Ӧģʽ1~15

void KEY_Init(void);//��ʼ��

		 				    
#endif
