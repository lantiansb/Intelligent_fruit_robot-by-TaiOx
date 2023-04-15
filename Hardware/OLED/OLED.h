#ifndef __OLED_H
#define __OLED_H

#include "ht32.h"
#include "ht32_board.h"
#include "ht32f5xxxx_i2c.h"
#include "delay.h"

#define OLED_MODE 0
#define SIZE 8
#define XLevelL		0x00
#define XLevelH		0x10
#define Max_Column	128
#define Max_Row		64
#define	Brightness	0xFF 
#define X_WIDTH 	128
#define Y_WIDTH 	64

#define OLED_CMD  0	//写命令
#define OLED_DATA 1	//写数据

//移植只需修改下宏定义
#define HTCFG_I2C_MASTER_CLK(ck)         (ck.Bit.I2C1)
#define HTCFG_I2C_MASTER_PORT            (HT_I2C1)
#define HTCFG_I2C_MASTER_SCL_GPIO_ID     (GPIO_PA)
#define HTCFG_I2C_MASTER_SCL_AFIO_PIN    (AFIO_PIN_0)
#define HTCFG_I2C_MASTER_SDA_GPIO_ID     (GPIO_PA)
#define HTCFG_I2C_MASTER_SDA_AFIO_PIN    (AFIO_PIN_1)

#define I2C_MASTER_ADDRESS  0x00
#define I2C_SLAVE_ADDRESS   0x3C
#define ClockSpeed          400000

void I2C_Configuration(void);
void OLED_WR_Byte(unsigned dat,unsigned cmd);  
void OLED_Display_On(void);
void OLED_Display_Off(void);	   							   		    
void OLED_Init(void);
void OLED_Clear(unsigned dat); 
void OLED_ShowChar(u8 x,u8 y,u8 chr,u8 Char_Size);
void OLED_ShowNum(u8 x,u8 y,u32 num,u8 len,u8 size);
void OLED_ShowString(u8 x,u8 y, u8 *p,u8 Char_Size);	 
void OLED_Set_Pos(unsigned char x, unsigned char y);
void OLED_ShowCHinese_TWO(u8 x,u8 y,u8 no);   
void OLED_ShowCHinese_Three(u8 x,u8 y,u8 no);
void OLED_P8x16String(u8 x,u8 y,u8 ch[]);
void OLED_DrawBMP(unsigned char x0, unsigned char y0,unsigned char x1, unsigned char y1,unsigned char BMP[]);
void fill_picture(unsigned char fill_Data);

#endif
