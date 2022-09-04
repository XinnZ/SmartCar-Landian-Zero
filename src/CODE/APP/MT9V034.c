/*
 * MT9V034.c
 *
 *  Created on: 2021��8��1��
 *      Author: XinnZ, Landian, HBUT
 *  XinnZ's Blog: https://blog.xinnz.cn/
 */

#include <APP/MT9V034.h>
#include <APP/Wireless.h>
#include "IfxDma.h"
#include "IfxScuEru.h"
#include "zf_stm_systick.h"
#include "zf_gpio.h"
#include "zf_eru.h"
#include "zf_eru_dma.h"

uint8 receive[3];
uint8 receive_num = 0;
vuint8 uart_receive_flag;

uint8 link_list_num;

uint8 *camera_buffer_addr;      //>����ͷ�����ַ


//>��Ҫ���õ�����ͷ������
int16 MT9V034_CFG[CONFIG_FINISH][2] =
{
        {AUTO_EXP,        0},       //>�Զ��ع�����      ��Χ1-63 0Ϊ�ر� ����Զ��ع⿪��  EXP_TIME�������õ����ݽ����Ϊ����ع�ʱ�䣬Ҳ�����Զ��ع�ʱ�������
                                    //>һ������ǲ���Ҫ����������ܣ���Ϊ�������ع���һ�㶼�ȽϾ��ȣ�����������߷ǳ������ȵ�������Գ������ø�ֵ������ͼ���ȶ���
        {EXP_TIME,      700},       //>�ع�ʱ��          ����ͷ�յ�����Զ����������ع�ʱ�䣬������ù���������Ϊ�������������ع�ֵ
        {FPS,           150},       //>ͼ��֡��          ����ͷ�յ�����Զ���������FPS���������������Ϊ������������FPS
        {SET_COL,   IMAGE_W},       //>ͼ��������        ��Χ1-752     K60�ɼ���������188
        {SET_ROW,   IMAGE_H},       //>ͼ��������        ��Χ1-480
        {LR_OFFSET,       0},       //>ͼ������ƫ����    ��ֵ ��ƫ��   ��ֵ ��ƫ��  ��Ϊ188 376 752ʱ�޷�����ƫ��    ����ͷ��ƫ�����ݺ���Զ��������ƫ�ƣ�������������ü�����������ƫ��
        {UD_OFFSET,       0},       //>ͼ������ƫ����    ��ֵ ��ƫ��   ��ֵ ��ƫ��  ��Ϊ120 240 480ʱ�޷�����ƫ��    ����ͷ��ƫ�����ݺ���Զ��������ƫ�ƣ�������������ü�����������ƫ��
        {GAIN,           32},       //>ͼ������          ��Χ16-64     ����������ع�ʱ��̶�������¸ı�ͼ�������̶�

        {INIT,            0}        //>����ͷ��ʼ��ʼ��
};

//>������ͷ�ڲ���ȡ������������
int16 GET_CFG[CONFIG_FINISH - 1][2] =
{
        {AUTO_EXP,  0},     //>�Զ��ع�����
        {EXP_TIME,  0},     //>�ع�ʱ��
        {FPS,       0},     //>ͼ��֡��
        {SET_COL,   0},     //>ͼ��������
        {SET_ROW,   0},     //>ͼ��������
        {LR_OFFSET, 0},     //>ͼ������ƫ����
        {UD_OFFSET, 0},     //>ͼ������ƫ����
        {GAIN,      0},     //>ͼ������
};

//>-------------------------------------------------------------------------------------------------------------------
//  @brief      MT9V034����ͷ�����жϺ���
//  @param      NULL
//  @return     void
//  @since      v1.0
//  Sample usage:
//  @note       �˺�����isr.c�� �������жϺ�������
//-------------------------------------------------------------------------------------------------------------------
void MT9V034_Uart_Callback(void)
{
    while (uart_query(MT9V034_COF_UART, &receive[receive_num]))
    {
        receive_num++;

        if (1 == receive_num && 0XA5 != receive[0])
            receive_num = 0;
        if (3 == receive_num)
        {
            receive_num = 0;
            uart_receive_flag = 1;
        }
    }
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      ��������ͷ�ڲ�������Ϣ
//  @param      uartn       ѡ��ʹ�õĴ���
//  @param      buff        ����������Ϣ�ĵ�ַ
//  @return     void
//  @since      v1.0
//  Sample usage:           ���øú���ǰ���ȳ�ʼ������
//-------------------------------------------------------------------------------------------------------------------
void set_config(UARTN_enum uartn, int16 buff[CONFIG_FINISH - 1][2])
{
    uint16 temp, i;
    uint8 send_buffer[4];

    uart_receive_flag = 0;

    //>���ò���  ������ο���������ֲ�
    //>��ʼ��������ͷ�����³�ʼ��
    for (i = 0; i < CONFIG_FINISH; i++)
    {
        send_buffer[0] = 0xA5;
        send_buffer[1] = (uint8)buff[i][0];
        temp = buff[i][1];
        send_buffer[2] = temp >> 8;
        send_buffer[3] = (uint8)temp;

        uart_putbuff(uartn, send_buffer, 4);
        systick_delay_ms(2);
    }
    //>�ȴ�����ͷ��ʼ���ɹ�
    while (!uart_receive_flag)
        ;
    uart_receive_flag = 0;
    while ((0xff != receive[1]) || (0xff != receive[2]))
        ;
    //>���ϲ��ֶ�����ͷ���õ�����ȫ�����ᱣ��������ͷ��51��Ƭ����eeprom��
    //>����set_exposure_time�����������õ��ع����ݲ��洢��eeprom��
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      ��ȡ����ͷ�ڲ�������Ϣ
//  @param      uartn       ѡ��ʹ�õĴ���
//  @param      buff        ����������Ϣ�ĵ�ַ
//  @return     void
//  @since      v1.0
//  Sample usage:           ���øú���ǰ���ȳ�ʼ������
//-------------------------------------------------------------------------------------------------------------------
void get_config(UARTN_enum uartn, int16 buff[CONFIG_FINISH - 1][2])
{
    uint16 temp, i;
    uint8 send_buffer[4];

    for (i = 0; i < CONFIG_FINISH - 1; i++)
    {
        send_buffer[0] = 0xA5;
        send_buffer[1] = GET_STATUS;
        temp = buff[i][0];
        send_buffer[2] = temp >> 8;
        send_buffer[3] = (uint8)temp;

        uart_putbuff(uartn, send_buffer, 4);

        //�ȴ����ܻش�����
        while (!uart_receive_flag)
            ;
        uart_receive_flag = 0;

        buff[i][1] = receive[1] << 8 | receive[2];
    }
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      ��ȡ����ͷ�̼��汾
//  @param      uartn       ѡ��ʹ�õĴ���
//  @return     void
//  @since      v1.0
//  Sample usage:           ���øú���ǰ���ȳ�ʼ������
//-------------------------------------------------------------------------------------------------------------------
uint16 get_version(UARTN_enum uartn)
{
    uint16 temp;
    uint8 send_buffer[4];
    send_buffer[0] = 0xA5;
    send_buffer[1] = GET_STATUS;
    temp = GET_VERSION;
    send_buffer[2] = temp >> 8;
    send_buffer[3] = (uint8)temp;

    uart_putbuff(uartn, send_buffer, 4);

    //>�ȴ����ܻش�����
    while (!uart_receive_flag)
        ;
    uart_receive_flag = 0;

    return ((uint16)(receive[1] << 8) | receive[2]);
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      ������������ͷ�ع�ʱ��
//  @param      uartn       ѡ��ʹ�õĴ���
//  @param      light       �����ع�ʱ��Խ��ͼ��Խ��������ͷ�յ������ݷֱ��ʼ�FPS��������ع�ʱ��������õ����ݹ�����ô����ͷ��������������ֵ
//  @return     uint16      ��ǰ�ع�ֵ������ȷ���Ƿ���ȷд��
//  @since      v1.0
//  Sample usage:           ���øú���ǰ���ȳ�ʼ������
//-------------------------------------------------------------------------------------------------------------------
uint16 set_exposure_time(UARTN_enum uartn, uint16 light)
{
    uint16 temp;
    uint8 send_buffer[4];

    send_buffer[0] = 0xA5;
    send_buffer[1] = SET_EXP_TIME;
    temp = light;
    send_buffer[2] = temp >> 8;
    send_buffer[3] = (uint8)temp;

    uart_putbuff(uartn, send_buffer, 4);

    //>�ȴ����ܻش�����
    while (!uart_receive_flag)
        ;
    uart_receive_flag = 0;

    temp = receive[1] << 8 | receive[2];
    return temp;
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      ������ͷ�ڲ��Ĵ�������д����
//  @param      uartn       ѡ��ʹ�õĴ���
//  @param      addr        ����ͷ�ڲ��Ĵ�����ַ
//  @param      data        ��Ҫд�������
//  @return     uint16      �Ĵ�����ǰ���ݣ�����ȷ���Ƿ�д��ɹ�
//  @since      v1.0
//  Sample usage:			���øú���ǰ���ȳ�ʼ������
//-------------------------------------------------------------------------------------------------------------------
uint16 set_mt9v034_reg(UARTN_enum uartn, uint8 addr, uint16 data)
{
    uint16 temp;
    uint8 send_buffer[4];

    send_buffer[0] = 0xA5;
    send_buffer[1] = SET_ADDR;
    temp = addr;
    send_buffer[2] = temp >> 8;
    send_buffer[3] = (uint8)temp;

    uart_putbuff(uartn, send_buffer, 4);
    systick_delay_ms(10);

    send_buffer[0] = 0xA5;
    send_buffer[1] = SET_DATA;
    temp = data;
    send_buffer[2] = temp >> 8;
    send_buffer[3] = (uint8)temp;

    uart_putbuff(uartn, send_buffer, 4);

    //>�ȴ����ܻش�����
    while (!uart_receive_flag)
        ;
    uart_receive_flag = 0;

    temp = receive[1] << 8 | receive[2];
    return temp;
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      MT9V034����ͷ��ʼ��
//  @param      NULL
//  @return     void
//  @since      v1.0
//  Sample usage:	ʹ��FLEXIO�ӿڲɼ�����ͷ
//-------------------------------------------------------------------------------------------------------------------
void MT9V034_Init(void)
{
    uint8 i;
    camera_buffer_addr = MT9V034.IMAGE[0];

    boolean interrupt_state = disableInterrupts();

    uart_init(MT9V034_COF_UART, 9600, MT9V034_COF_UART_TX, MT9V034_COF_UART_RX); //>��ʼ������ ��������ͷ
    enableInterrupts();                                                          //>�����ж�

    //>�ȴ�����ͷ�ϵ��ʼ���ɹ� ��ʽ������ ��ʱ����ͨ����ȡ���õķ�ʽ ��ѡһ
    //systick_delay_ms(500);                //>��ʱ��ʽ
    get_config(MT9V034_COF_UART, GET_CFG);  //>��ȡ���õķ�ʽ

    uart_receive_flag = 0;
    set_config(MT9V034_COF_UART, MT9V034_CFG);

    //>��ȡ���ñ��ڲ鿴�����Ƿ���ȷ
    get_config(MT9V034_COF_UART, GET_CFG);

    disableInterrupts();

    //>����ͷ�ɼ���ʼ��
    //>��ʼ�� ��������
    for (i = 0; i < 8; i++)
    {
        gpio_init((PIN_enum)(MT9V034_DATA_PIN + i), GPI, 0, PULLUP);
    }

    link_list_num = eru_dma_init(MT9V034_DMA_CH, GET_PORT_IN_ADDR(MT9V034_DATA_PIN), camera_buffer_addr, MT9V034_PCLK_PIN, FALLING, IMAGE_W * IMAGE_H);

    eru_init(MT9V034_VSYNC_PIN, FALLING);   //>��ʼ�����жϣ�������Ϊ�½��ش����ж�
    restoreInterrupts(interrupt_state);
}

uint8 MT9V034_dma_int_num;  //>��ǰDMA�жϴ���
//-------------------------------------------------------------------------------------------------------------------
//  @brief      MT9V034����ͷ���ж�
//  @param      NULL
//  @return     void
//  @since      v1.0
//  Sample usage:				�˺�����isr.c�б�eru��GPIO�жϣ��жϵ���
//-------------------------------------------------------------------------------------------------------------------
void MT9V034_Vsync(void)
{
    CLEAR_GPIO_FLAG(MT9V034_VSYNC_PIN);
    MT9V034_dma_int_num = 0;
    if (!MT9V034.FINISH)    //>�鿴ͼ�������Ƿ�ʹ����ϣ����δʹ������򲻿�ʼ�ɼ���������ַ��ʳ�ͻ
    {
        if (1 == link_list_num)
        {
            //>û�в������Ӵ���ģʽ ��������Ŀ�ĵ�ַ
            DMA_SET_DESTINATION(MT9V034_DMA_CH, camera_buffer_addr);
        }
        dma_start(MT9V034_DMA_CH);
    }
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      MT9V034����ͷDMA����ж�
//  @param      NULL
//  @return     void
//  @since      v1.0
//  Sample usage:				�˺�����isr.c�б�dma�жϵ���
//-------------------------------------------------------------------------------------------------------------------
void MT9V034_DMA(void)
{
    CLEAR_DMA_FLAG(MT9V034_DMA_CH);
    MT9V034_dma_int_num++;

    if (MT9V034_dma_int_num >= link_list_num)
    {
        //>�ɼ����
        MT9V034_dma_int_num = 0;
        MT9V034.FINISH      = 1;    //>һ��ͼ��Ӳɼ���ʼ���ɼ�������ʱ3.8MS����(50FPS��188*120�ֱ���)
        dma_stop(MT9V034_DMA_CH);
    }
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      ���������ͷͼ��������λ���鿴ͼ��
//  @param      image			��Ҫ���͵�ͼ���ַ
//  @param      width			ͼ�����
//  @param      height			ͼ�����
//  @return     void
//  @since      v1.0
//  Sample usage:
//-------------------------------------------------------------------------------------------------------------------
void MT9V034_Send_IMG(uint8 *image, uint16 width, uint16 height)
{
    uint8 cmd[4] = {0x00, 0xff, 0x01, 0x01};    //>����
    Wireless_Send(cmd, 4);                 //>��������
    Wireless_Send(image, width * height);  //>����ͼ��
}
