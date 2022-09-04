/*
 * RUN.h
 *
 *  Created on: 2021��8��1��
 *      Author: XinnZ, Landian, HBUT
 *  XinnZ's Blog: https://blog.xinnz.cn/
 */

#ifndef CODE_ALGO_RUN_H_
#define CODE_ALGO_RUN_H_

#include "common.h"
#include "headfile.h"

typedef struct
{
    bool Mode_CAP;  // ��С����

    bool FirstCharge;  // ��һ�γ��
    bool Charged;      // ������
    bool ToCharge;     // ��Ҫ���
    bool ChargeReady;  // ������

    uint8 IndexV;     // �ٶȾ�������
    bool FindTarget;  // �ҵ��ű��
} Flag_t;

typedef struct
{
    float TatgetP;   // ��������λ���⻷P
    int32 UphillT;   // ���¼���ƫ��
    float ChatgeP;   // ���λ���⻷P
    int32 UphillC;   // ����ƫ��(���)
    float AngleP;    // �ǶȻ�P
    int32 AngleCOR;  // �Ƕ�����
    float OmegaP;    // ���ٶȻ�P
} Stage_t;

void RUN_Init(void);
void RUN_GoGoGo(void);

#endif /* CODE_ALGO_RUN_H_ */
