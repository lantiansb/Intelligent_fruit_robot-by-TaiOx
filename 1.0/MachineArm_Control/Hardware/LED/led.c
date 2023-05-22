#include "led.h"

void LED_Init() //LED初始化函数
{
	
    HT_GPIO_TypeDef* GPIO_GROUP;
    GPIO_GROUP = HT_GPIOC; 
    AFIO_GPxConfig(LED_GPIO_GROUP, LED1_PIN|LED2_PIN, LED_AFIO_MODE);	
    GPIO_DirectionConfig(GPIO_GROUP,LED1_PIN|LED2_PIN, GPIO_DIR_OUT);
    GPIO_PullResistorConfig(GPIO_GROUP,LED1_PIN|LED2_PIN, GPIO_PR_DISABLE);      //上拉电阻
    GPIO_DriveConfig(GPIO_GROUP,LED1_PIN|LED2_PIN, GPIO_DV_8MA);                 //选择指定GPIO-PIN的驱动电流，可选GPIO_DV_4/8/12/16MA

}
