#include "ht32.h"
#include "sys.h"
#include "led.h"
#include "delay.h"
#include "key.h"
#include "usart.h"
#include "Track.h"
#include "PWM.h"
#include "GPIOinput.h"
#include "delay.h"

extern u8 turn_flag;

int main(void)
{
    USART_Configuration();
    printf("hhhhh\r\n");
    delay_ms(2000);
    Track_Init();
    Go();
    while(1)
    {
        if(turn_flag)
        {
            Car_CTRL();
        }
        else
        {
//            Go();
//            delay_ms(1000);
            turn_right();
            delay_ms(1000);
            turn_flag = 1;
        }
    }
}
