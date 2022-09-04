/*
 * TwoPass.h
 *
 *  Created on: 2021��8��1��
 *      Author: XinnZ, Landian, HBUT
 *  XinnZ's Blog: https://blog.xinnz.cn/
 *        Link: https://blog.csdn.net/icvpr/article/details/10259577
 */

#ifndef CODE_ALGO_TWOPASS_H_
#define CODE_ALGO_TWOPASS_H_

#include <APP/MT9V034.h>

#include "common.h"

typedef struct CON_Node {
    uint8 nsX;     // ��ͨ����ʼ����x
    uint8 nsY;     // ��ͨ����ʼ����y
    uint8 neX;     // ��ͨ���������x
    uint8 neY;     // ��ͨ���������y
    uint8 nArea;   // ��ͨ�����
    uint8 nEdge;   // ��ͨ��߳�
    float nRound;  // ��ͨ��Բ�ζ�
} CON_Node;

typedef struct
{
    uint16 IMAGE[IMAGE_H][IMAGE_W];  // ��ͨ��ͼ������
    uint16 EDGES[IMAGE_H][IMAGE_W];  // ��ͨ��߽�����
    volatile uint16 count;           // ��ͨ�����
} CON_t;
extern CON_t CONV;

void CON_TwoPass(const MT9V034_t camera, CON_t *con);

#endif /* CODE_ALGO_TWOPASS_H_ */
