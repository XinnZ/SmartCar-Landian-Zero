/*
 * UI.h
 *
 *  Created on: 2021��8��1��
 *      Author: XinnZ, Landian, HBUT
 *  XinnZ's Blog: https://blog.xinnz.cn/
 */

#ifndef CODE_BSP_UI_H_
#define CODE_BSP_UI_H_

typedef enum
{
    ui_Main,    //>������
    ui_Motor,   //>������Խ���
    ui_MT9V034, //>����ͷ���Խ���
    ui_Modules, //>�ⲿģ����Խ���
    ui_RUN      //>�ܳ�����
} UI_e;

typedef struct
{
    unsigned char CurrentUI;    //>��ǰ����
    unsigned char LastFocus;    //>�ϴξ۽�����
    unsigned char CurrentFocus; //>��ǰ�۽�����
} UI_Focus_t;                   //>����۽�
extern UI_Focus_t UI_Focus;

void UI_Init(void);
void UI_Charge(void);
void UI_Main(void);
void UI_Motor(void);
void UI_MT9V034(void);
void UI_Modules(void);

#endif /* CODE_BSP_UI_H_ */
