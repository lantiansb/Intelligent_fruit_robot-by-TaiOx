
#ifndef _LED_H
#define _LED_H
 
#include "sys.h"


#define LED_GPIO_GROUP             (GPIO_PC)
#define LED1_PIN                   (GPIO_PIN_14)
#define LED2_PIN                   (GPIO_PIN_15)
#define LED_AFIO_MODE              (AFIO_MODE_DEFAULT) //默认模式：AFIO_MODE_DEFAULT ，AFIO_MODE_1~15对应模式1~15
void LED_Init(void);

#endif

