//��Ƭ��ͷ�ļ�
#include "ht32.h"

//�����豸����
#include "K210.h"
#include "led.h"

//Ӳ������
#include "delay.h"
#include "usart.h"
//C��
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define REV_OK		0	//������ɱ�־
#define REV_WAIT	1	//����δ��ɱ�־

unsigned char location_buf[20];
unsigned short K210_cnt = 0, K210_cntPre = 0;//�������������ڻ�ȡ��ǰ����״̬

//==========================================================
//	�������ƣ�	K210_Clear
//
//	�������ܣ�	��ջ���
//
//	��ڲ�����	��
//
//	���ز�����	��
//
//	˵����		
//==========================================================
void K210_Clear(void)
{
	memset(location_buf, 0, sizeof(location_buf));
	K210_cnt = 0;
}

//==========================================================
//	�������ƣ�	K210_WaitRecive
//
//	�������ܣ�	�ȴ��������
//
//	��ڲ�����	��
//
//	���ز�����	REV_OK-�������		REV_WAIT-���ճ�ʱδ���
//
//	˵����		ѭ�����ü���Ƿ�������
//==========================================================
_Bool K210_WaitRecive(void)
{

	if(K210_cnt == 0) 							//������ռ���Ϊ0 ��˵��û�д��ڽ��������У�����ֱ����������������
		return REV_WAIT;
		
	if(K210_cnt == K210_cntPre)				//�����һ�ε�ֵ�������ͬ����˵���������
	{
		K210_cnt = 0;							//��0���ռ���
			
		return REV_OK;								//���ؽ�����ɱ�־
	}
		
	K210_cntPre = K210_cnt;					//��Ϊ��ͬ
	
	return REV_WAIT;								//���ؽ���δ��ɱ�־

}

//==========================================================
//	�������ƣ�	K210_SendCmd
//
//	�������ܣ�	����ʶ��ĳĳˮ������
//
//	��ڲ�����	cmd�����1-��ɫ��2-��ɫ
//
//	���ز�����	��
//
//	˵����		
//==========================================================
void K210_SendCmd(char *cmd)
{

	Usart_SendString(HT_UART0, (unsigned char *)cmd, strlen((const char *)cmd));	
}

//==========================================================
//	�������ƣ�	K210_GetIPD
//
//	�������ܣ�	��ȡK210���ص��������겢����Ϊ��������
//
//	��ڲ�����	�ȴ���ʱ��(����10ms)
//
//	���ز�����	K210���ص���������
//
//	˵����		��������
//==========================================================
float *K210_GetIPD(unsigned short timeOut)
{
    float location_xyz[3];//���ض�Ӧ���������xyzֵ
	char ptrIPD[10];
    char *token, *next_token;
    const char delimiters[] = ","; // ����ָ���
	strcpy(ptrIPD, (const char*)location_buf);
	do
	{
        token = strtok_r(ptrIPD, delimiters, &next_token);
        if (token != NULL) 
        {
            location_xyz[1] = (160-atof(token))/40*2.5;//��������ת�����������������y��ֵ
            token = strtok_r(NULL, delimiters, &next_token);
            if (token != NULL) 
            {
                location_xyz[0] = (160-atof(token))/40*3.0 + 21;//�����������x��ֵ
                K210_Clear();
                printf("x: %.2f, y: %.2f\r\n", location_xyz[0]+1, location_xyz[1]);
            }
            location_xyz[2] = -2;
            return location_xyz;
        }
		delay_ms(5);													//��ʱ�ȴ�
	} while(timeOut--);
	
	return NULL;														//��ʱ��δ�ҵ������ؿ�ָ��
}

//==========================================================
//	�������ƣ�	UART0_IRQHandler
//
//	�������ܣ�	����0�����ж�
//
//	��ڲ�����	��
//
//	���ز�����	��
//
//	˵����		
//==========================================================
void UART0_IRQHandler(void)
{

	if(USART_GetFlagStatus(HT_UART0, USART_FLAG_RXDR) != RESET) //�����ж�
	{
		if(K210_cnt >= sizeof(location_buf))	K210_cnt = 0; //��ֹ���ڱ�ˢ��
		location_buf[K210_cnt++] = HT_UART0->DR;
		USART_ClearFlag(HT_UART0, USART_FLAG_RSADD);
	}

}

