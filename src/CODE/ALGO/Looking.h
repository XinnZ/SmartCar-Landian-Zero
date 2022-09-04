/*
 * Looking.h
 *
 *  Created on: 2021��8��1��
 *      Author: XinnZ, Landian, HBUT
 *  XinnZ's Blog: https://blog.xinnz.cn/
 */

#ifndef CODE_ALGO_LOOKING_H_
#define CODE_ALGO_LOOKING_H_

#include "../APP/MT9V034.h"
#include "TwoPass.h"
#include "common.h"

typedef struct
{
    _Bool EUREKA;  // �ҵ�Ŀ���־λ
    uint8 X;       // Ŀ�����ĺ�����
    uint8 Y;       // Ŀ������������
    uint8 SoftX;   // Ŀ�����ĺ����� ��ʹ���
    uint8 SoftY;   // Ŀ������������ ��ʹ���

    uint8 sX;      // Ŀ����ͨ����ʼ����x
    uint8 sY;      // Ŀ����ͨ����ʼ����y
    uint8 eX;      // Ŀ����ͨ���������x
    uint8 eY;      // Ŀ����ͨ���������y
    uint8 cArea;   // Ŀ����ͨ�����
    uint8 cEdge;   // Ŀ����ͨ��߳�
    float cRound;  // Ŀ����ͨ��Բ�ζ�
    uint8 count;   // ���������������ͨ�����
} Looking_t;
extern Looking_t Looking;
extern _Bool Mode_Undistort;
extern _Bool Mode_Looking;

void Looking_Target(Looking_t *look, MT9V034_t camera, CON_t con);
void Looking_Coordinates(Looking_t *look);
void Image_Undistort_MAP(void);
void Image_Undistort(MT9V034_t *cam);
void Image_BIN(MT9V034_t *cam);
void Image_Threshold(MT9V034_t *cam);
void Image_thresholdGray2Binary(MT9V034_t *cam);

#endif /* CODE_ALGO_LOOKING_H_ */
