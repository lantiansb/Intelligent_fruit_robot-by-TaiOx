/*���޸���־��
1-2020/02/16 ����
2-2020/02/17 ����ö�ٶ���
3-2020/02/17 ȥ��ÿ�ζ�Ҫ����ʱ�ӵĺ���
*/

#include "led.h"
 

 
void LED_Init() //led��ʼ������
{
	
	 HT_GPIO_TypeDef* GPIO_GROUP;                                                 //ö��
   GPIO_GROUP = HT_GPIOC;                                                       //�Ժ�ֻҪ�޸����Ｔ�� 
 	 AFIO_GPxConfig(LED_GPIO_GROUP, LED1_PIN|LED2_PIN, LED_AFIO_MODE);            //����AFIOģʽ	
   GPIO_DirectionConfig(GPIO_GROUP,LED1_PIN|LED2_PIN, GPIO_DIR_OUT);            //����GPIOģʽ��GPIO_DIR_OUT���orGPIO_DIR_IN����                                                                              */
   GPIO_PullResistorConfig(GPIO_GROUP,LED1_PIN|LED2_PIN, GPIO_PR_DISABLE);      //�������裬ʹIO��Ϊ�ߵ�ƽ
	 GPIO_DriveConfig(GPIO_GROUP,LED1_PIN|LED2_PIN, GPIO_DV_8MA);                 //ѡ��ָ��GPIO���ŵ�������������ѡGPIO_DV_4/8/12/16MA 

}

