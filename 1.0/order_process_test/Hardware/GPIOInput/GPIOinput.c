#include "ht32.h"
#include "ht32_board.h"
#include "ht32_board_config.h"

FlagStatus TmpStatus = SET;				//PB12
FlagStatus TmpStatus1 = SET;			//PD1
FlagStatus TmpStatus2 = SET;			//PD2
void GPIOInout_CKCU_Configuration(void)
{
#if 1
  CKCU_PeripClockConfig_TypeDef CKCUClock = {{0}};
  CKCUClock.Bit.PC = 1;
  CKCUClock.Bit.PB = 1;
  CKCUClock.Bit.PD = 1;
  CKCUClock.Bit.AFIO   = 1;
  CKCU_PeripClockConfig(CKCUClock, ENABLE);
	
		 GPIO_DirectionConfig(HT_GPIOC, GPIO_PIN_14,   GPIO_DIR_OUT);      //配置GPIO口LED
     GPIO_DirectionConfig(HT_GPIOC, GPIO_PIN_15, GPIO_DIR_OUT);
		 GPIO_DirectionConfig(HT_GPIOC, GPIO_PIN_1, GPIO_DIR_OUT);
#endif
}


void GPIO_IN_Configuration(void)
{
  /* Configure WAKEUP, KEY1, KEY2 pins as the input function                                                */
  /* Configure AFIO mode of input pins                                                                      */
  AFIO_GPxConfig(GPIO_PB, AFIO_PIN_12, AFIO_FUN_GPIO);
  AFIO_GPxConfig(GPIO_PD, AFIO_PIN_1, AFIO_FUN_GPIO);
  AFIO_GPxConfig(GPIO_PD, AFIO_PIN_2, AFIO_FUN_GPIO);

  /* Configure GPIO direction of input pins      输入模式                                                           */
  GPIO_DirectionConfig(HT_GPIOB, GPIO_PIN_12, GPIO_DIR_IN);
  GPIO_DirectionConfig(HT_GPIOD, GPIO_PIN_1, GPIO_DIR_IN);
  GPIO_DirectionConfig(HT_GPIOD, GPIO_PIN_2, GPIO_DIR_IN);

  /* Configure GPIO pull resistor of input pins  引脚浮空                                                           */
  GPIO_PullResistorConfig(HT_GPIOB, GPIO_PIN_12, GPIO_PR_UP);
  GPIO_PullResistorConfig(HT_GPIOD, GPIO_PIN_1, GPIO_PR_UP);
  GPIO_PullResistorConfig(HT_GPIOD, GPIO_PIN_2, GPIO_PR_UP);

  GPIO_InputConfig(HT_GPIOB, GPIO_PIN_12, ENABLE);
  GPIO_InputConfig(HT_GPIOD, GPIO_PIN_1, ENABLE);
  GPIO_InputConfig(HT_GPIOD, GPIO_PIN_2, ENABLE);
}




void GPIO_MainRoutine(void)
{
                                                                
  TmpStatus = GPIO_ReadInBit(HT_GPIOB, GPIO_PIN_12);
  GPIO_WriteOutBits(HT_GPIOC, GPIO_PIN_14, TmpStatus);
                                                                
  TmpStatus1 = GPIO_ReadInBit(HT_GPIOD, GPIO_PIN_1);
  GPIO_WriteOutBits(HT_GPIOC, GPIO_PIN_15, TmpStatus1);
                                                                   
  TmpStatus2 = GPIO_ReadInBit(HT_GPIOD, GPIO_PIN_8);
  GPIO_WriteOutBits(HT_GPIOC, GPIO_PIN_1, TmpStatus2);
}
