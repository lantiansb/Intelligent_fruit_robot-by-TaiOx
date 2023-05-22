#ifndef _LED_H
#define _LED_H
 
#include "sys.h"

#define LED_GPIO_GROUP             (GPIO_PD)
#define LED1_PIN                   (GPIO_PIN_8)
#define LED2_PIN                   (GPIO_PIN_7)
#define LED_AFIO_MODE              (AFIO_MODE_DEFAULT) //默认模式：AFIO_MODE_DEFAULT, AFIO_MODE_!~15对应模式1~15
void LED_Init(void);

#endif
