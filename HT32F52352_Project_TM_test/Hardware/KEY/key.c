/*���޸���־��
1-2020/02/16 ����
2-2020/02/17 ����ö�ٶ���
3-2020/02/17 ȥ��ÿ�ζ�Ҫ����ʱ�ӵĺ���
*/


#include "key.h"

void KEY_Init(void){ 
	
	 HT_GPIO_TypeDef* GPIO_GROUP;                                                 //ö��
   GPIO_GROUP = HT_GPIOA;                                                       //�Ժ�ֻҪ�޸����Ｔ��                                    
	 AFIO_GPxConfig(KEY_GPIO_GROUP, KEY1_PIN|KEY2_PIN, KEY_AFIO_MODE);            //����AFIOģʽ	
	 GPIO_InputConfig(GPIO_GROUP,KEY1_PIN|KEY2_PIN, ENABLE);	                     //����ʹ�ܺ������˺�����ʵ��GPIO�ڱ�Ϊ����ģʽ���������裬Ĭ�ϵ������������������滻����������ע�͵ĺ���
//  GPIO_DirectionConfig(GPIO_GROUP,KEY1_PIN|KEY2_PIN, GPIO_DIR_IN);             //����GPIOģʽ��GPIO_DIR_OUT���orGPIO_DIR_IN����                                                                             */
//  GPIO_PullResistorConfig(GPIO_GROUP,KEY1_PIN|KEY2_PIN, GPIO_PR_UP);           //�������裬ʹIO��Ϊ�ߵ�ƽ
//	GPIO_DriveConfig(GPIO_GROUP,LED1_PIN|LED2_PIN, GPIO_DV_4MA);                 //ѡ��ָ��GPIO���ŵ�������������ѡGPIO_DV_4/8/12/16MA 


}
 

