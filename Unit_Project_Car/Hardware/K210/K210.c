//单片机头文件
#include "ht32.h"

//网络设备驱动
#include "K210.h"
#include "led.h"

//硬件驱动
#include "delay.h"
#include "usart.h"
//C库
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define REV_OK		0	//接收完成标志
#define REV_WAIT	1	//接收未完成标志

unsigned char location_buf[20];
unsigned short K210_cnt = 0, K210_cntPre = 0;//辅助变量，用于获取当前接收状态

//==========================================================
//	函数名称：	K210_Clear
//
//	函数功能：	清空缓存
//
//	入口参数：	无
//
//	返回参数：	无
//
//	说明：		
//==========================================================
void K210_Clear(void)
{
	memset(location_buf, 0, sizeof(location_buf));
	K210_cnt = 0;
}

//==========================================================
//	函数名称：	K210_WaitRecive
//
//	函数功能：	等待接收完成
//
//	入口参数：	无
//
//	返回参数：	REV_OK-接收完成		REV_WAIT-接收超时未完成
//
//	说明：		循环调用检测是否接收完成
//==========================================================
_Bool K210_WaitRecive(void)
{

	if(K210_cnt == 0) 							//如果接收计数为0 则说明没有处于接收数据中，所以直接跳出，结束函数
		return REV_WAIT;
		
	if(K210_cnt == K210_cntPre)				//如果上一次的值和这次相同，则说明接收完毕
	{
		K210_cnt = 0;							//清0接收计数
			
		return REV_OK;								//返回接收完成标志
	}
		
	K210_cntPre = K210_cnt;					//置为相同
	
	return REV_WAIT;								//返回接收未完成标志

}

//==========================================================
//	函数名称：	K210_SendCmd
//
//	函数功能：	发送识别某某水果命令
//
//	入口参数：	cmd：命令，1-橘色，2-绿色
//
//	返回参数：	无
//
//	说明：		
//==========================================================
void K210_SendCmd(char *cmd)
{

	Usart_SendString(HT_UART0, (unsigned char *)cmd, strlen((const char *)cmd));	
}

//==========================================================
//	函数名称：	K210_GetIPD
//
//	函数功能：	获取K210返回的像素坐标并处理为世界坐标
//
//	入口参数：	等待的时间(乘以10ms)
//
//	返回参数：	K210返回的坐标数据
//
//	说明：		解码数据
//==========================================================
float *K210_GetIPD(unsigned short timeOut)
{
    float location_xyz[3];//返回对应世界坐标的xyz值
	char ptrIPD[10];
    char *token, *next_token;
    const char delimiters[] = ","; // 定义分隔符
	strcpy(ptrIPD, (const char*)location_buf);
	do
	{
        token = strtok_r(ptrIPD, delimiters, &next_token);
        if (token != NULL) 
        {
            location_xyz[1] = (160-atof(token))/40*2.5;//像素坐标转换，并对世界坐标的y赋值
            token = strtok_r(NULL, delimiters, &next_token);
            if (token != NULL) 
            {
                location_xyz[0] = (160-atof(token))/40*3.0 + 21;//对世界坐标的x赋值
                K210_Clear();
                printf("x: %.2f, y: %.2f\r\n", location_xyz[0]+1, location_xyz[1]);
            }
            location_xyz[2] = -2;
            return location_xyz;
        }
		delay_ms(5);													//延时等待
	} while(timeOut--);
	
	return NULL;														//超时还未找到，返回空指针
}

//==========================================================
//	函数名称：	UART0_IRQHandler
//
//	函数功能：	串口0接收中断
//
//	入口参数：	无
//
//	返回参数：	无
//
//	说明：		
//==========================================================
void UART0_IRQHandler(void)
{

	if(USART_GetFlagStatus(HT_UART0, USART_FLAG_RXDR) != RESET) //接收中断
	{
		if(K210_cnt >= sizeof(location_buf))	K210_cnt = 0; //防止串口被刷爆
		location_buf[K210_cnt++] = HT_UART0->DR;
		USART_ClearFlag(HT_UART0, USART_FLAG_RSADD);
	}

}

