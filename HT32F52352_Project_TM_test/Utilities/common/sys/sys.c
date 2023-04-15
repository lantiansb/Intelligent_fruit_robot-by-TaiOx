/*《修改日志》
1-2020/02/16 创建
2-2020/02/17 优化每次需要开两次时钟的问题
*/

#include "sys.h"

void NVIC_Configuration(void){ //嵌套中断向量控制器 的设置
    //NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	//设置NVIC中断分组2:2位抢占优先级，2位响应优先级
}

void RCC_Configuration(void){ //RCC时钟设置  
	
	CKCU_PeripClockConfig_TypeDef CCLOCK = {{0}};//不开启外设时钟相应功能无法使用
	CCLOCK.Bit.PA    = 1;//开启PA时钟
	CCLOCK.Bit.PB    = 0;//开启PB时钟
	CCLOCK.Bit.PC    = 1;//开启PC时钟
	CCLOCK.Bit.PD    = 0;//开启PD时钟
	CCLOCK.Bit.USART1 = 1;//开启usart串口1
	CCLOCK.Bit.AFIO  = 1;//开启复用功能时钟
	CKCU_PeripClockConfig(CCLOCK, ENABLE);//使能时钟
}
