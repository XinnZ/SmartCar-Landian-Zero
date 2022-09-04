/*
 * IPS114.h
 *
 *  Created on: 2021��8��1��
 *      Author: XinnZ, Landian, HBUT
 *  XinnZ's Blog: https://blog.xinnz.cn/
 */

#ifndef CODE_APP_IPS114_H_
#define CODE_APP_IPS114_H_

#include "common.h"

#define IPS114_SPIN_PIN     SPI_0               //>����ʹ�õ�SPI��
#define IPS114_SCL_PIN      SPI0_SCLK_P20_11    //>����SPI_SCK����
#define IPS114_SDA_PIN      SPI0_MOSI_P20_14    //>����SPI_MOSI����
#define IPS114_SDA_IN_PIN   SPI0_MISO_P20_12    //>����SPI_MISO����  IPSû��MISO���ţ�����������Ȼ��Ҫ���壬��spi�ĳ�ʼ��ʱ��Ҫʹ��
#define IPS114_CS_PIN       SPI0_CS2_P20_13     //>����SPI_CS����

#define IPS114_BL_PIN       P20_12              //>Һ���������Ŷ��� ����ʵ��ͨѶδʹ��P15_4��� ���︴��ΪGPIO
#define IPS114_REST_PIN     P15_1               //>Һ����λ���Ŷ���
#define IPS114_DC_PIN       P15_0               //>Һ������λ���Ŷ���

#define IPS114_BL(x)        gpio_set(IPS114_BL_PIN, x)
#define IPS114_DC(x)        gpio_set(IPS114_DC_PIN, x)
#define IPS114_REST(x)      gpio_set(IPS114_REST_PIN, x)

//���� ����ʽ �ֿ�16 �ָ�16 ����

//16λBMP 40X40 QQͼ��ȡģ����
//Image2LCDȡģѡ������
//ˮƽɨ��
//16λ
//40X40
//������ͼ��ͷ����
//��������
//�Զ�����
//��λ��ǰ

#define Color_WHITE     0xFFFF      //>��ɫ
#define Color_BLACK     0x0000      //>��ɫ
#define Color_RED       0xF800      //>��ɫ
#define Color_BLUE      0x001F      //>��ɫ
#define Color_YELLOW    0xFFE0      //>��ɫ
#define Color_GREEN     0x07E0      //>��ɫ
#define Color_GRAY      0X8430      //>��ɫ
#define Color_BROWN     0XBC40      //>��ɫ
#define Color_PURPLE    0XF81F      //>��ɫ
#define Color_PINK      0XFE19      //>��ɫ

#define IPS114_PENCOLOR Color_WHITE //>����д�ֱʵ���ɫ
#define IPS114_BGCOLOR  Color_BLACK //>���屳����ɫ

#define IPS114_W 135
#define IPS114_H 240

//������ʾ����
//0 ����ģʽ
//1 ����ģʽ  ��ת180
//2 ����ģʽ
//3 ����ģʽ  ��ת180
#define IPS114_DISPLAY_DIR 3

#if (0 == IPS114_DISPLAY_DIR || 1 == IPS114_DISPLAY_DIR)
#define IPS114_X_MAX IPS114_W //Һ��X�����
#define IPS114_Y_MAX IPS114_H //Һ��Y�����

#elif (2 == IPS114_DISPLAY_DIR || 3 == IPS114_DISPLAY_DIR)
#define IPS114_X_MAX IPS114_H //Һ��X�����
#define IPS114_Y_MAX IPS114_W //Һ��Y�����

#else
#error "IPS114_DISPLAY_DIR �������"

#endif

void IPS114_Init(void);
void IPS114_Clear(uint16 color);
void IPS114_Draw_Point(uint16 x, uint16 y, uint16 color);
void IPS114_Draw_Part(uint16 xs, uint16 ys, uint16 xe, uint16 ye, uint16 color_dat);
void IPS114_Draw_Rectangle(uint16 xs, uint16 ys, uint16 xe, uint16 ye, uint16 color_dat);
void IPS114_Char(uint16 x, uint16 y, const int8 dat, uint16 word_color, uint16 back_color);
void IPS114_String(uint16 x, uint16 y, const int8 dat[], uint16 word_color, uint16 back_color);
void IPS114_IMAGE032(uint8 *p, uint16 width, uint16 height);
void IPS114_IMAGE032_BIN(uint8 *p, uint8 threshold);
void IPS114_IMAGE032_CON(uint16 *p, uint16 total);
//void IPS114_Chinese(uint16 x, uint16 y, uint8 size, const uint8 *p, uint8 number, uint16 color);

#endif
