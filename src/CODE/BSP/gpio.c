/*
 * gpio.c
 *
 *  Created on: 2021��8��1��
 *      Author: XinnZ, Landian, HBUT
 *  XinnZ's Blog: https://blog.xinnz.cn/
 */

#include <BSP/gpio.h>
#include "zf_gpio.h"

uint8 Mode[2][4];
volatile uint8 Key;

/*
 * @brief   : GPIO_Init  GPIO��ʼ��
 * @param   : None
 * @return  : None
 * @example : None
 * @author  : XinnZ
 */
void GPIO_Init(void)
{
    gpio_init(LED0,     GPO,    1,  PUSHPULL);  //>���İ�LED0
    gpio_init(LED1,     GPO,    1,  PUSHPULL);  //>���İ�LED1
    gpio_init(LED2,     GPO,    1,  PUSHPULL);  //>���İ�LED2
    gpio_init(LED3,     GPO,    1,  PUSHPULL);  //>���İ�LED3

    gpio_init(KEY0,     GPI,    1,  PULLUP);    //>���ذ�����1
    gpio_init(KEY1,     GPI,    1,  PULLUP);    //>���ذ�����2
    gpio_init(KEY2,     GPI,    1,  PULLUP);    //>���ذ�����3
    gpio_init(KEY3,     GPI,    1,  PULLUP);    //>���ذ�����4
    gpio_init(KEY4,     GPI,    1,  PULLUP);    //>���ذ�����5

    gpio_init(DIP0,     GPI,    1,  PULLUP);    //>���ذ�DIP0
    gpio_init(DIP1,     GPI,    1,  PULLUP);    //>���ذ�DIP1
    gpio_init(DIP2,     GPI,    1,  PULLUP);    //>���ذ�DIP2
    gpio_init(DIP3,     GPI,    1,  PULLUP);    //>���ذ�DIP3
    gpio_init(DIP4,     GPI,    1,  PULLUP);    //>���ذ�DIP4
    gpio_init(DIP5,     GPI,    1,  PULLUP);    //>���ذ�DIP5
    gpio_init(DIP6,     GPI,    1,  PULLUP);    //>���ذ�DIP6
    gpio_init(DIP7,     GPI,    1,  PULLUP);    //>���ذ�DIP7

    gpio_init(WL_RTSp,  GPI,    0,  PULLDOWN);  //>���ذ����ߴ�����������
}

/*
 * @brief   : Key_Scan  ��ȡ���򿪹�
 * @param   : None
 * @return  : None
 * @example : None
 * @author  : XinnZ
 */
void Key_Scan(void)
{
    static _Bool Flag_Loos = 1; //>�����������δ���
    uint8 Value;

    Value = (gpio_get(KEY0)     |  (gpio_get(KEY1)<<1) |
            (gpio_get(KEY2)<<2) |  (gpio_get(KEY3)<<3) |
            (gpio_get(KEY4)<<4) );

    switch (Value)
    {
        case 0x1f:  Key = Key_None;     break;
        case 0x0f:  Key = Key_Up;       break;
        case 0x1b:  Key = Key_Down;     break;
        case 0x17:  Key = Key_Left;     break;
        case 0x1e:  Key = Key_Right;    break;
        case 0x1d:  Key = Key_Press;    break;
        default:    Key = Key_None;     break;
    }

    if (Key != Key_None)
    {
        if (!Flag_Loos)
        {
            Key = Key_None;
        }
        Flag_Loos = 0;
    }
    else
        Flag_Loos = 1;
}
