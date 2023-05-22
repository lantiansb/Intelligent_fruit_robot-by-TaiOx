#include "key.h"

void KEY_Init(void)
{
    HT_GPIO_TypeDef* GPIO_GROUP;                                                 //枚举
    GPIO_GROUP = HT_GPIOA;                                                       //移植仅修改此处                                    
    AFIO_GPxConfig(KEY_GPIO_GROUP, KEY1_PIN | KEY2_PIN, KEY_AFIO_MODE);            //配置AFIO模式
    GPIO_InputConfig(GPIO_GROUP, KEY1_PIN | KEY2_PIN, ENABLE);	                     //输入使能函数，此函数可实现GPIO口变为输入模式，上拉电阻，默认电流，可替换下三函数
//  GPIO_DirectionConfig(GPIO_GROUP,KEY1_PIN|KEY2_PIN, GPIO_DIR_IN);             //配置GPIO模式(输入或输出)                                                                             */
//  GPIO_PullResistorConfig(GPIO_GROUP,KEY1_PIN|KEY2_PIN, GPIO_PR_UP);           //上拉电阻，是IO口为高电平
//	GPIO_DriveConfig(GPIO_GROUP,LED1_PIN|LED2_PIN, GPIO_DV_4MA);                 //选择指定GPIO引脚的驱动电流，可选GPIO_DV_4/8/12/16mA 

}
