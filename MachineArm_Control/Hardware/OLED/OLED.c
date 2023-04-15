#include "OLED.h"
#include "OLED_Font.h"

/*引脚初始化*/
void I2C_Configuration(void)
{
    I2C_InitTypeDef  I2C_InitStructure;
    I2C_DeInit(HTCFG_I2C_MASTER_PORT);
    {
        /* Enable peripheral clock                                                                              */
        CKCU_PeripClockConfig_TypeDef CKCUClock = {{0}};
        HTCFG_I2C_MASTER_CLK(CKCUClock) = 1;
        CKCUClock.Bit.AFIO              = 1;
        CKCU_PeripClockConfig(CKCUClock, ENABLE);
    }

        /* Configure GPIO to I2C mode for Master                                                                  */
        AFIO_GPxConfig(HTCFG_I2C_MASTER_SCL_GPIO_ID, HTCFG_I2C_MASTER_SCL_AFIO_PIN, AFIO_FUN_I2C);
        AFIO_GPxConfig(HTCFG_I2C_MASTER_SDA_GPIO_ID, HTCFG_I2C_MASTER_SDA_AFIO_PIN, AFIO_FUN_I2C);

    { /* I2C Master configuration                                                                             */

        /* !!! NOTICE !!!
            Notice that the local variable (structure) did not have an initial value.
            Please confirm that there are no missing members in the parameter settings below in this function.
        */

        I2C_InitStructure.I2C_GeneralCall = DISABLE;
        I2C_InitStructure.I2C_AddressingMode = I2C_ADDRESSING_7BIT;
        I2C_InitStructure.I2C_Acknowledge = ENABLE;
        I2C_InitStructure.I2C_OwnAddress = I2C_MASTER_ADDRESS;
        I2C_InitStructure.I2C_Speed = ClockSpeed;
        I2C_InitStructure.I2C_SpeedOffset = 0;
        I2C_Init(HTCFG_I2C_MASTER_PORT, &I2C_InitStructure);
    }
    I2C_Cmd(HTCFG_I2C_MASTER_PORT, ENABLE);
}

/*********************************************************************************************************//**
  * @brief  OLED acknowledge polling.
  * @retval None
  ***********************************************************************************************************/
void I2C_OLED_AckPolling(void)
{
  u32 reg;

  /* wait if bus busy                                                                                      */
  while (I2C_GetFlagStatus(HTCFG_I2C_MASTER_PORT, I2C_FLAG_BUSBUSY));
 
  while (1)
  {
    /* send slave address                                                                                   */
    I2C_TargetAddressConfig(HTCFG_I2C_MASTER_PORT, I2C_SLAVE_ADDRESS, I2C_MASTER_WRITE);

    /* check status                                                                                         */
    while (1)
    {
      reg = HTCFG_I2C_MASTER_PORT->SR;

      if (reg & I2C_FLAG_ADRS)
      {
        return;
      }

      if (reg & I2C_FLAG_RXNACK)
      {
        I2C_ClearFlag(HTCFG_I2C_MASTER_PORT, I2C_FLAG_RXNACK);
        break;
      }
    }
  }
}

void I2C_WriteByte(uint8_t addr,uint8_t data)		
{
    /* OLED acknowledge polling                                                                             */
    I2C_OLED_AckPolling();

    /* Check on Master Transmitter TXDE condition                                                           */
    while (!I2C_CheckStatus(HTCFG_I2C_MASTER_PORT, I2C_MASTER_TX_EMPTY));    
    /* Master Send I2C addr                                                                                 */
    I2C_SendData(HTCFG_I2C_MASTER_PORT, addr);
    
    /* Check on Master Transmitter TXDE condition                                                           */
    while (!I2C_CheckStatus(HTCFG_I2C_MASTER_PORT, I2C_MASTER_TX_EMPTY));
    /* Master Send I2C data                                                                                 */
    I2C_SendData(HTCFG_I2C_MASTER_PORT, data);

    /* end of write                                                                                           */
    while (!I2C_CheckStatus(EEPROM_I2C, I2C_MASTER_TX_EMPTY));
    I2C_GenerateSTOP(EEPROM_I2C);
}

//IIC写指令函数
void WriteCmd(uint8_t I2C_Command)
{
	I2C_WriteByte(0x00, I2C_Command);	//从0x00开始写指令	
}

//IIC写数据函数
void WriteDat(uint8_t I2C_Data)
{
	I2C_WriteByte(0x40, I2C_Data);	//从0x40开始写数据
}

void OLED_WR_Byte(unsigned dat,unsigned cmd)
{
	if(cmd)
			{

   WriteDat(dat);
   
		}
	else {
   WriteCmd(dat);
		
	}


}


/********************************************
// fill_Picture
********************************************/
void fill_picture(unsigned char fill_Data)
{
	unsigned char m,n;
	for(m=0;m<8;m++)
	{
		OLED_WR_Byte(0xb0+m,0);		//page0-page1
		OLED_WR_Byte(0x00,0);		//low column start address
		OLED_WR_Byte(0x10,0);		//high column start address
		for(n=0;n<128;n++)
			{
				OLED_WR_Byte(fill_Data,1);
			}
	}
}


/***********************Delay****************************************/
void Delay_50ms(unsigned int Del_50ms)
{
	unsigned int m;
	for(;Del_50ms>0;Del_50ms--)
		for(m=6245;m>0;m--);
}

void Delay_1ms(unsigned int Del_1ms)
{
	unsigned char j;
	while(Del_1ms--)
	{	
		for(j=0;j<123;j++);
	}
}

//坐标设置
void OLED_Set_Pos(unsigned char x, unsigned char y) 
{ 	OLED_WR_Byte(0xb0+y,OLED_CMD);
	OLED_WR_Byte(((x&0xf0)>>4)|0x10,OLED_CMD);
	OLED_WR_Byte((x&0x0f),OLED_CMD); 
}

//开启OLED显示    
void OLED_Display_On(void)
{
	OLED_WR_Byte(0X8D,OLED_CMD);  //OLED显示
	OLED_WR_Byte(0X14,OLED_CMD);  //DCDC 
	OLED_WR_Byte(0XAF,OLED_CMD);  //DISPLAY ON
}

//关闭OLED显示     
void OLED_Display_Off(void)
{
	OLED_WR_Byte(0X8D,OLED_CMD);  //SET DCDCÃüÁî
	OLED_WR_Byte(0X10,OLED_CMD);  //DCDC OFF
	OLED_WR_Byte(0XAE,OLED_CMD);  //DISPLAY OFF
}

//清屏函数，清完屏，整个屏幕都是黑的，与没电两用一样	  
void OLED_Clear(unsigned dat)  
{  
	u8 i,n;		    
	for(i=0;i<8;i++)  
	{  
		OLED_WR_Byte (0xb0+i,OLED_CMD);    //设置页地址(0~7)
		OLED_WR_Byte (0x00,OLED_CMD);      //设置显示位置——列低地址
		OLED_WR_Byte (0x10,OLED_CMD);      //设置显示位置——列高地址   
		for(n=0;n<128;n++)OLED_WR_Byte(dat,OLED_DATA); 
	} //更新显示
}
void OLED_On(void)  
{  
	u8 i,n;		    
	for(i=0;i<8;i++)  
	{  
		OLED_WR_Byte (0xb0+i,OLED_CMD);    //设置页地址(0~7)
		OLED_WR_Byte (0x00,OLED_CMD);      //设置显示位置——列低地址
		OLED_WR_Byte (0x10,OLED_CMD);      //设置显示位置——列高地址   
		for(n=0;n<128;n++)OLED_WR_Byte(1,OLED_DATA); 
	} //更新显示
}
//在指定位置显示一个字符，包括部分字符
//x:0~127
//y:0~63
//mode:0，反白显示；1，正常显示				 
//size:字体选择 16/12 
void OLED_ShowChar(u8 x,u8 y,u8 chr,u8 Char_Size)
{      	
	unsigned char c=0,i=0;	
		c=chr-' ';//µÃµ½Æ«ÒÆºóµÄÖµ			
		if(x>Max_Column-1){x=0;y=y+2;}
		if(Char_Size ==16)
			{
			OLED_Set_Pos(x,y);	
			for(i=0;i<8;i++)
			OLED_WR_Byte(F8X16[c*16+i],OLED_DATA);
			OLED_Set_Pos(x,y+1);
			for(i=0;i<8;i++)
			OLED_WR_Byte(F8X16[c*16+i+8],OLED_DATA);
			}
			else {	
				OLED_Set_Pos(x,y);
				for(i=0;i<6;i++)
				OLED_WR_Byte(F6x8[c][i],OLED_DATA);
				
			}
}

//m^n函数
u32 oled_pow(u8 m,u8 n)
{
	u32 result=1;	 
	while(n--)result*=m;    
	return result;
}
	  
//显示2个数字
//x,y :起点坐标	 
//len :数字的位数
//size:字体大小
//mode:模式0，填充模式；模式1，叠加模式
//num:数值(0~4294967295);	 		  
void OLED_ShowNum(u8 x,u8 y,u32 num,u8 len,u8 size2)
{         	
	u8 t,temp;
	u8 enshow=0;						   
	for(t=0;t<len;t++)
	{
		temp=(num/oled_pow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				OLED_ShowChar(x+(size2/2)*t,y,' ',size2);
				continue;
			}else enshow=1; 
		 	 
		}
	 	OLED_ShowChar(x+(size2/2)*t,y,temp+'0',size2); 
	}
} 
//ÏÔÊ¾Ò»¸ö×Ö·ûºÅ´®
void OLED_ShowString(u8 x,u8 y,u8 *chr,u8 Char_Size)
{
	unsigned char j=0;
	while (chr[j]!='\0')
	{		OLED_ShowChar(x,y,chr[j],Char_Size);
			x+=8;
		if(x>120){x=0;y+=2;}
			j++;
	}
}




//@

void OLED_P8x16String(u8 x,u8 y,u8 ch[])
{
	u8 c=0,i=0,j=0;
	while (ch[j]!='\0')
	{
		c =ch[j]-32;
		if(x>120){x=0;y++;}
		OLED_Set_Pos(x,y);
		for(i=0;i<8;i++)
			OLED_WR_Byte(F8X16[c*16+i],OLED_DATA);
		OLED_Set_Pos(x,y+1);
		for(i=0;i<8;i++)
			OLED_WR_Byte(F8X16[c*16+i+8],OLED_DATA);
		x+=8;
		j++;
	}
}


/****************************字库****/

//显示汉字
void OLED_ShowCHinese_TWO(u8 x,u8 y,u8 no)
{      			    
	u8 t,adder=0;
	OLED_Set_Pos(x,y);	
    for(t=0;t<16;t++)
		{
				OLED_WR_Byte(Date[2*no][t],OLED_DATA);
				adder+=1;
     }	
		OLED_Set_Pos(x,y+1);	
    for(t=0;t<16;t++)
			{	
				OLED_WR_Byte(Date[2*no+1][t],OLED_DATA);
				adder+=1;
      }					
}



void OLED_ShowCHinese_Three(u8 x,u8 y,u8 no)
{      			    
	u8 t,adder=0;
	OLED_Set_Pos(x,y);	
    for(t=0;t<16;t++)
		{
				OLED_WR_Byte(Temp[2*no][t],OLED_DATA);
				adder+=1;
     }	
		OLED_Set_Pos(x,y+1);	
    for(t=0;t<16;t++)
			{	
				OLED_WR_Byte(Temp[2*no+1][t],OLED_DATA);
				adder+=1;
      }					
}


void OLED_ShowCHinese_four(u8 x,u8 y,u8 no)
{      			    
	u8 t,adder=0;
	OLED_Set_Pos(x,y);	
    for(t=0;t<16;t++)
		{
				OLED_WR_Byte(F16x16[2*no][t],OLED_DATA);
				adder+=1;
     }	
		OLED_Set_Pos(x,y+1);	
    for(t=0;t<16;t++)
			{	
				OLED_WR_Byte(F16x16[2*no+1][t],OLED_DATA);
				adder+=1;
      }					
}




/***********显示BMP图片128*64，起点坐标(x,y),x范围（0~127），y范围（0~7）*****************/
void OLED_DrawBMP(unsigned char x0, unsigned char y0,unsigned char x1, unsigned char y1,unsigned char BMP[])
{ 	
 unsigned int j=0;
 unsigned char x,y;
  
  if(y1%8==0) y=y1/8;      
  else y=y1/8+1;
	for(y=y0;y<y1;y++)
	{
		OLED_Set_Pos(x0,y);
    for(x=x0;x<x1;x++)
	    {      
	    	OLED_WR_Byte(BMP[j++],OLED_DATA);	    	
	    }
	}
} 

//³õÊ¼»¯SSD1306					    
void OLED_Init(void)
{ 	
    OLED_WR_Byte(0xAE,OLED_CMD);//--display off
	OLED_WR_Byte(0x00,OLED_CMD);//---set low column address
	OLED_WR_Byte(0x10,OLED_CMD);//---set high column address
	OLED_WR_Byte(0x40,OLED_CMD);//--set start line address  
	OLED_WR_Byte(0xB0,OLED_CMD);//--set page address
	OLED_WR_Byte(0x81,OLED_CMD); // contract control
	OLED_WR_Byte(0xFF,OLED_CMD);//--128   
	OLED_WR_Byte(0xA1,OLED_CMD);//set segment remap 
	OLED_WR_Byte(0xA6,OLED_CMD);//--normal / reverse
	OLED_WR_Byte(0xA8,OLED_CMD);//--set multiplex ratio(1 to 64)
	OLED_WR_Byte(0x3F,OLED_CMD);//--1/32 duty
	OLED_WR_Byte(0xC8,OLED_CMD);//Com scan direction
	OLED_WR_Byte(0xD3,OLED_CMD);//-set display offset
	OLED_WR_Byte(0x00,OLED_CMD);//
	
	OLED_WR_Byte(0xD5,OLED_CMD);//set osc division
	OLED_WR_Byte(0x80,OLED_CMD);//
	
	OLED_WR_Byte(0xD8,OLED_CMD);//set area color mode off
	OLED_WR_Byte(0x05,OLED_CMD);//
	
	OLED_WR_Byte(0xD9,OLED_CMD);//Set Pre-Charge Period
	OLED_WR_Byte(0xF1,OLED_CMD);//
	
	OLED_WR_Byte(0xDA,OLED_CMD);//set com pin configuartion
	OLED_WR_Byte(0x12,OLED_CMD);//
	
	OLED_WR_Byte(0xDB,OLED_CMD);//set Vcomh
	OLED_WR_Byte(0x30,OLED_CMD);//
	
	OLED_WR_Byte(0x8D,OLED_CMD);//set charge pump enable
	OLED_WR_Byte(0x14,OLED_CMD);//
	
	OLED_WR_Byte(0xAF,OLED_CMD);//--turn on oled panel
}  

