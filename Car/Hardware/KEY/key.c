/*《修改日志》
1-2020/02/16 创建
2-2020/02/17 增加枚举定义
3-2020/02/17 去除每次都要开启时钟的函数
*/


#include "key.h"

void KEY_Init(void){ 
	
	 HT_GPIO_TypeDef* GPIO_GROUP;                                                 //枚举
   GPIO_GROUP = HT_GPIOA;                                                       //以后只要修改这里即可                                    
	 AFIO_GPxConfig(KEY_GPIO_GROUP, KEY1_PIN|KEY2_PIN, KEY_AFIO_MODE);            //配置AFIO模式	
	 GPIO_InputConfig(GPIO_GROUP,KEY1_PIN|KEY2_PIN, ENABLE);	                     //输入使能函数，此函数可实现GPIO口变为输入模式，上拉电阻，默认电流。因此这个函数可替换以下三个被注释的函数
//  GPIO_DirectionConfig(GPIO_GROUP,KEY1_PIN|KEY2_PIN, GPIO_DIR_IN);             //配置GPIO模式：GPIO_DIR_OUT输出orGPIO_DIR_IN输入                                                                             */
//  GPIO_PullResistorConfig(GPIO_GROUP,KEY1_PIN|KEY2_PIN, GPIO_PR_UP);           //上拉电阻，使IO口为高电平
//	GPIO_DriveConfig(GPIO_GROUP,LED1_PIN|LED2_PIN, GPIO_DV_4MA);                 //选择指定GPIO引脚的驱动电流，可选GPIO_DV_4/8/12/16MA 


}
 

