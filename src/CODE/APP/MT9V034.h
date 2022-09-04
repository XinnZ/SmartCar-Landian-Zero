/*
 * MT9V034.h
 *
 *  Created on: 2021��8��1��
 *      Author: XinnZ, Landian, HBUT
 *  XinnZ's Blog: https://blog.xinnz.cn/
 */

#ifndef CODE_APP_MT9V034_H_
#define CODE_APP_MT9V034_H_

#include "common.h"
#include "IfxDma_cfg.h"
#include "zf_uart.h"

//>��������ͷ����
#define IMAGE_W             162         //>ͼ���� ��Χ1-188
#define IMAGE_H             100         //>ͼ��߶� ��Χ1-120
#define IMAGE_SIZE          IMAGE_W * IMAGE_H     //>ͼ��С�� 65536/4 ��ֹͼ�����
#define IMAGE_MIDDLE        IMAGE_W / 2

#define IMAGE_ignoreTop     15          //>ͼ������Զ�������
#define IMAGE_ignoreBottom  0           //>ͼ������Եײ�����

//>������ڲκͻ������ ��������MATLAB
#define DIS_K1      -0.3030930f         //>�������
#define DIS_K2      0.07450939f
#define DIS_P1      0.0f                //>�������
#define DIS_P2      0.0f;
#define DIS_Fx      71.5098269f         //>ʹ������������x�᷽�򽹾�ĳ���
#define DIS_Fy      71.5781746f         //>ʹ������������y�᷽�򽹾�ĳ���
#define DIS_Cx      78.2887089f
#define DIS_Cy      40.5534325f

typedef struct
{
    _Bool FINISH;                       //>һ��ͼ��ɼ���ɱ�־λ
    uint8 IMAGE[IMAGE_H][IMAGE_W];      //>ͼ��ԭʼ����   ����4�ֽڶ���
    uint8 UNDST[IMAGE_H][IMAGE_W];      //>ͼ��ȥ�������� ����4�ֽڶ���
    uint8 BIN[IMAGE_H][IMAGE_W];        //>ͼ���ֵ������ ����4�ֽڶ���
    uint8 threshold;                    //>��ֵ
    uint8 threshold_offset;             //>��ֵ����
} MT9V034_t;
extern MT9V034_t MT9V034;

//--------------------------------------------------------------------------------------------------
//>��������
//--------------------------------------------------------------------------------------------------
#define MT9V034_COF_UART UART_1 //>��������ͷ��ʹ�õ��Ĵ���
#define MT9V034_COF_UART_TX UART1_TX_P11_12
#define MT9V034_COF_UART_RX UART1_RX_P11_10

#define MT9V034_VSYNC_PIN ERU_CH3_REQ6_P02_0    //>���ж����� ��ѡ��Χ�ο�ERU_PIN_enumö�� ��������������ѡ��Ϊͬһ��ͨ���������ǹ����жϵ�ͨ�� \
                                                //>���糡�ж�ѡ��ERU_CH3 ��ô�������žͲ���ѡ��ERU_CH7����Ϊ3��7���ж��ǹ��õġ�

#define MT9V034_DATA_PIN P00_0                  //>����D0��������  ����D0����ΪP00_0 ��ôD1��ʹ�õ�������ΪP00_1���������� \
                                                //>�����ò���P00_0��P02_0��P15_0����������Ϊ��������

#define MT9V034_PCLK_PIN ERU_CH2_REQ2_P10_2     //>��������ʱ������ ��ѡ��Χ�ο�ERU_PIN_enumö�� �����볡�ж�����ѡ��Ϊͬһ��ͨ���������ǹ����жϵ�ͨ�� \
                                                //>���糡�ж�ѡ��ERU_CH3 ��ô�������žͲ���ѡ��ERU_CH7����Ϊ3��7���ж��ǹ��õġ�

#define MT9V034_DMA_CH IfxDma_ChannelId_5       //>����ʹ�õ�DMAͨ�� 0-47��ѡ  ͨ����Խ�����ȼ�Խ��

//>����ͷ����ö��
typedef enum
{
    INIT = 0,      //>����ͷ��ʼ������
    AUTO_EXP,      //>�Զ��ع�����
    EXP_TIME,      //>�ع�ʱ������
    FPS,           //>����ͷ֡������
    SET_COL,       //>ͼ��������
    SET_ROW,       //>ͼ��������
    LR_OFFSET,     //>ͼ������ƫ������
    UD_OFFSET,     //>ͼ������ƫ������
    GAIN,          //>ͼ��ƫ������
    CONFIG_FINISH, //>������λ����Ҫ����ռλ����

    COLOR_GET_WHO_AM_I = 0xEF,
    SET_EXP_TIME = 0XF0, //>���������ع�ʱ������
    GET_STATUS,          //>��ȡ����ͷ��������
    GET_VERSION,         //>�̼��汾������

    SET_ADDR = 0XFE, //>�Ĵ�����ַ����
    SET_DATA         //>�Ĵ�����������
} CMD;

extern uint8 receive[3];         //>�����ڽ���������Ϣʱ����  ��ֹ�û�ʹ�øñ���
extern uint8 receive_num;        //>�����ڽ���������Ϣʱ����  ��ֹ�û�ʹ�øñ���
extern vuint8 uart_receive_flag; //>�����ڽ���������Ϣʱ����  ��ֹ�û�ʹ�øñ���

void set_config(UARTN_enum uartn, int16 buff[CONFIG_FINISH - 1][2]);
void get_config(UARTN_enum uartn, int16 buff[CONFIG_FINISH - 1][2]);
uint16 get_version(UARTN_enum uartn);
uint16 set_exposure_time(UARTN_enum uartn, uint16 light);

void MT9V034_Init(void);
void MT9V034_Vsync(void);
void MT9V034_DMA(void);
void MT9V034_Uart_Callback(void);
void MT9V034_Send_IMG(uint8 *image, uint16 width, uint16 height);

#endif
