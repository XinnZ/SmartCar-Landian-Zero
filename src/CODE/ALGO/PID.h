/*
 * PID.h
 *
 *  Created on: 2021��8��1��
 *      Author: XinnZ, Landian, HBUT
 *  XinnZ's Blog: https://blog.xinnz.cn/
 */

#ifndef CODE_ALGO_PID_H_
#define CODE_ALGO_PID_H_

#include "common.h"

typedef struct
{
    float kP;  // P
    float kI;  // I
    float kD;  // D

    int32 integral_limit;   // �����޷�
    float integrator;       // ����ֵ ����ۼ�
    int32 last_error;       // �ϴ����
    int32 last_derivative;  // �ϴ���������ϴ����֮��
} PID_t;
extern PID_t PID_Angle, PID_Omega, PID_Velocity_R, PID_Velocity_L, PID_Location;

void PID_Init(PID_t *pid);
int32 PID_Limiter(int32 amt, int32 low, int32 high);
int32 PID_Position(PID_t *pid, int32 actual, int32 set);
int32 PID_Increase(PID_t *pid, int32 actual, int32 set);

#endif /* CODE_ALGO_PID_H_ */
