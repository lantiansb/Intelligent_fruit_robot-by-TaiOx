#include "usart.h"
#include "ht32f5xxxx_gpio.h"


/**************************ʵ�ֺ���********************************************
����˵��������usart����

*******************************************************************************/ 
void USART_Configuration(void)
{
	 USART_InitTypeDef USART_InitStructure;
	 {                                                              
     CKCU_PeripClockConfig_TypeDef CKCUClock = {{0}};
     CKCUClock.Bit.AFIO                   = 1;
     CKCUClock.Bit.PA = 1;			
     CKCUClock.Bit.USART1         = 1;			
     CKCU_PeripClockConfig(CKCUClock, ENABLE);
    }
  
	{                                                          
		 CKCU_PeripClockConfig_TypeDef CKCUClock = {{0}};
		 CKCUClock.Bit.AFIO                   = 1;
		 CKCUClock.Bit.PA = 1;			
		 CKCUClock.Bit.USART0         = 1;			
		 CKCU_PeripClockConfig(CKCUClock, ENABLE);
	}
	 
  GPIO_PullResistorConfig(HT_GPIOA, GPIO_PIN_15, GPIO_PR_UP);
	GPIO_PullResistorConfig(HT_GPIOA, GPIO_PIN_3, GPIO_PR_UP);

  AFIO_GPxConfig(GPIO_PA, AFIO_PIN_14, AFIO_FUN_USART_UART);
  AFIO_GPxConfig(GPIO_PA, AFIO_PIN_15, AFIO_FUN_USART_UART);
	AFIO_GPxConfig(GPIO_PA, AFIO_PIN_2, AFIO_FUN_USART_UART);
	AFIO_GPxConfig(GPIO_PA, AFIO_PIN_3, AFIO_FUN_USART_UART);

  /*
		�����ʣ� 115200
		���ȣ�   8bits
		ֹͣλ�� 1λ
	  У��λ�� ��			
	  ģʽ��   ����ģʽ
  */
		
  USART_InitStructure.USART_BaudRate = 115200;
  USART_InitStructure.USART_WordLength = USART_WORDLENGTH_8B;
  USART_InitStructure.USART_StopBits = USART_STOPBITS_1;
  USART_InitStructure.USART_Parity = USART_PARITY_NO;
  USART_InitStructure.USART_Mode = USART_MODE_NORMAL;
  USART_Init(HT_USART1, &USART_InitStructure);
	USART_Init(HT_USART0, &USART_InitStructure);
  
		 //�ж�����    
		NVIC_EnableIRQ(USART1_IRQn);
		NVIC_EnableIRQ(USART0_IRQn);
			
		USART_IntConfig(HT_USART1, USART_INT_RXDR, ENABLE);
		USART_IntConfig(HT_USART0, USART_INT_RXDR, ENABLE);
			
		// ʹ�� COM1_PORT  ���ͺͽ��� 
		USART_TxCmd(HT_USART1, ENABLE);
		USART_RxCmd(HT_USART1, ENABLE);
		USART_TxCmd(HT_USART0, ENABLE);
		USART_RxCmd(HT_USART0, ENABLE);
	

	 	                                                                                      
//  USART_RXTLConfig(COM1_PORT, USART_RXTL_04); ����FIFO���յȼ�

}


/**************************ʵ�ֺ���********************************************
����˵���������жϷ�����

*******************************************************************************/ 
void USART1_IRQHandler(void)			//����1�ж�
{
	u8 data;
	
	if( USART_GetFlagStatus(HT_USART1, USART_FLAG_RXDR ) )         //�����ж�
	{
		data = USART_ReceiveData(HT_USART1);                         //��ȡ���յ�������
		printf("data = %c\n",data);                                  //���յ������ݷ��ͻص���	
	}																																//���Ͷ��ֽڻ��ν����ж�
	USART_ClearFlag(HT_USART1,USART_FLAG_RXDR);
}

void USART0_IRQHandler(void)			//����1�ж�
{
	u8 data;
	
	if( USART_GetFlagStatus(HT_USART0, USART_FLAG_RXDR ) )         //�����ж�
	{
		data = USART_ReceiveData(HT_USART0);                         //��ȡ���յ�������
		Usart_Sendbyte(HT_USART0,data);                                 //���յ������ݷ��ͻص���		
	}																																//���Ͷ��ֽڻ��ν����ж�
	USART_ClearFlag(HT_USART0,USART_FLAG_RXDR);
}



void USART_Tx1(const char* TxBuffer, u32 length)
{
  int i;

  for (i = 0; i < length; i++)
  {
    while (!USART_GetFlagStatus(HT_USART1, USART_FLAG_TXC));
    USART_SendData(HT_USART1, TxBuffer[i]);
  }
}
void USART_Tx0(const char* TxBuffer, u32 length)
{
  int i;

  for (i = 0; i < length; i++)
  {
    while (!USART_GetFlagStatus(HT_USART0, USART_FLAG_TXC));
    USART_SendData(HT_USART0, TxBuffer[i]);
  }
}


/**************************ʵ�ֺ���********************************************
����˵��������һ���ֽ�

*******************************************************************************/ 
void Usart_Sendbyte(HT_USART_TypeDef* USARTx, u8 data)
{
	USART_SendData(USARTx, data);
	while (USART_GetFlagStatus(USARTx, USART_FLAG_TXDE) == RESET);		
}
 
/**************************ʵ�ֺ���********************************************
����˵������������

*******************************************************************************/ 
void Usart_SendArray(HT_USART_TypeDef* USARTx, u8 *array,u8 num)
{
	u8 i;
	for( i = 0;i < num;i++)
	{
		Usart_Sendbyte(USARTx,*array);
		array++;
	}
}
 /**************************ʵ�ֺ���********************************************
����˵���������ַ���

*******************************************************************************/ 

void Usart_SendStr(HT_USART_TypeDef* USARTx, uint8_t *str)
{
	uint8_t i;
	for(i = 0;str[i] != '\0';i++)
	{
		Usart_Sendbyte(USARTx,str[i]);
	}
}
