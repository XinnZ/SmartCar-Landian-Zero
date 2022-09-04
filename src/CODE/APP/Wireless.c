/*
 * Wireless.c
 *
 *  Created on: 2021��8��1��
 *      Author: XinnZ, Landian, HBUT
 *  XinnZ's Blog: https://blog.xinnz.cn/
 */

#include <APP/Wireless.h>
#include <BSP/gpio.h>
#include "TC377_config.h"
#include "zf_stm_systick.h"
#include "zf_uart.h"
#include "zf_gpio.h"

uint8 wireless_rx_buffer;
//-------------------------------------------------------------------------------------------------------------------
//--> @brief      ����ת����ģ�� �����жϺ���
//--> @param      void
//--> @return     void
//--> Sample usage:
//--> @note       �ú�����ISR�ļ� ����2�жϳ��򱻵���
//-------------------------------------------------------------------------------------------------------------------
void Wireless_uart_callback(void)
{
    while (uart_query(DEBUG_UART, &wireless_rx_buffer))
        ;
    //-->��ȡ�յ�����������
}

//-------------------------------------------------------------------------------------------------------------------
//--> @brief      ����ת����ģ�� ���ͺ���
//--> @param      buff        ��Ҫ���͵����ݵ�ַ
//--> @param      len         ���ͳ���
//--> @return     uint32      ʣ��δ���͵��ֽ���
//--> Sample usage:
//--> @note
//-------------------------------------------------------------------------------------------------------------------
uint32 Wireless_Send(uint8 *buff, uint32 len)
{
    while (len > 30)
    {
        if (gpio_get(WL_RTSp))
        {
            return len; //-->ģ��æ,�������ǰ����ʹ��while�ȴ� �����ʹ�ú���ע�͵�while�ȴ�����滻��if���
        }
        //while(gpio_get(RTS_PIN)); //-->���RTSΪ�͵�ƽ���������������
        uart_putbuff(DEBUG_UART, buff, 30);

        buff += 30; //-->��ַƫ��
        len -= 30;  //-->����
    }

    if (gpio_get(WL_RTSp))
    {
        return len; //-->ģ��æ,�������ǰ����ʹ��while�ȴ� �����ʹ�ú���ע�͵�while�ȴ�����滻��if���
    }
    //while(gpio_get(RTS_PIN)); //-->���RTSΪ�͵�ƽ���������������
    uart_putbuff(DEBUG_UART, buff, len); //-->������������

    return 0;
}

/*
 * @brief   : VOFA_Send()   ����λ����������
 * @param   : data1
 *            data2
 * @return  : None
 * @example : None
 * @author  : XinnZ
 */
void VOFA_Send(float data1, float data2, float data3)
{
    float datas[3];
    uint8 data[16];
    datas[0] = data1;
    datas[1] = data2;
    datas[2] = data3;

    memcpy(data, (uint8 *)datas, sizeof(datas));

    data[12] = 0x00;
    data[13] = 0x00;
    data[14] = 0x80;
    data[15] = 0x7f;

    Wireless_Send(data, 16);
}
