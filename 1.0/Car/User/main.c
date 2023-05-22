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
    Track_Init();
    Go();
    while(1)
    {
        Car_CTRL();
    }
}
