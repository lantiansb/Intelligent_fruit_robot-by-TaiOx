#include "sys.h"

void NVIC_Configuration(void){ //嵌套中断向量控制器的设置 
    //NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	//设置NVIC中断分组的
}

void RCC_Configuration(void){ //RCC时钟设置  
	
	CKCU_PeripClockConfig_TypeDef CCLOCK = {{0}};//不开启外设时钟，0初始化时钟union
	CCLOCK.Bit.PA    = 1;//开启PA时钟
	CCLOCK.Bit.PB    = 0;//开启PB时钟
	CCLOCK.Bit.PC    = 1;//开启PC时钟
	CCLOCK.Bit.PD    = 0;//开启PC时钟
	CCLOCK.Bit.USART1 = 1;//开启usart串口1时钟
	CCLOCK.Bit.AFIO  = 1;//开启复用功能时钟
	CKCU_PeripClockConfig(CCLOCK, ENABLE);//导入结构体，使能初始化
}
