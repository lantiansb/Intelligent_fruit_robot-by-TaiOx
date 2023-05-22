/*《修改日志》
1-2020/02/16 创建
2-2020/02/17 增加枚举定义
3-2020/02/17 去除每次都要开启时钟的函数
*/

#include "led.h"
 

 
void LED_Init() //led初始化函数
{
	
	 HT_GPIO_TypeDef* GPIO_GROUP;                                                 //枚举
   GPIO_GROUP = HT_GPIOC;                                                       //以后只要修改这里即可 
 	 AFIO_GPxConfig(LED_GPIO_GROUP, LED1_PIN|LED2_PIN, LED_AFIO_MODE);            //配置AFIO模式	
   GPIO_DirectionConfig(GPIO_GROUP,LED1_PIN|LED2_PIN, GPIO_DIR_OUT);            //配置GPIO模式：GPIO_DIR_OUT输出orGPIO_DIR_IN输入                                                                              */
   GPIO_PullResistorConfig(GPIO_GROUP,LED1_PIN|LED2_PIN, GPIO_PR_DISABLE);      //上拉电阻，使IO口为高电平
	 GPIO_DriveConfig(GPIO_GROUP,LED1_PIN|LED2_PIN, GPIO_DV_8MA);                 //选择指定GPIO引脚的驱动电流，可选GPIO_DV_4/8/12/16MA 

}

