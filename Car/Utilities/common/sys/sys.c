/*���޸���־��
1-2020/02/16 ����
2-2020/02/17 �Ż�ÿ����Ҫ������ʱ�ӵ�����
*/

#include "sys.h"

void NVIC_Configuration(void){ //Ƕ���ж����������� ������
    //NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	//����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
}

void RCC_Configuration(void){ //RCCʱ������  
	
	CKCU_PeripClockConfig_TypeDef CCLOCK = {{0}};//����������ʱ����Ӧ�����޷�ʹ��
	CCLOCK.Bit.PA    = 1;//����PAʱ��
	CCLOCK.Bit.PB    = 0;//����PBʱ��
	CCLOCK.Bit.PC    = 1;//����PCʱ��
	CCLOCK.Bit.PD    = 0;//����PDʱ��
	CCLOCK.Bit.USART1 = 1;//����usart����1
	CCLOCK.Bit.AFIO  = 1;//�������ù���ʱ��
	CKCU_PeripClockConfig(CCLOCK, ENABLE);//ʹ��ʱ��
}
