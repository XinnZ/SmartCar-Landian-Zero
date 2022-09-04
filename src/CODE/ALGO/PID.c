/*
 * PID.c
 *
 *  Created on: 2021��8��1��
 *      Author: XinnZ, Landian, HBUT
 *  XinnZ's Blog: https://blog.xinnz.cn/
 */

#include "PID.h"

/**
 * @brief  : Limiting
 * @param  : amt:
 * @param  : low:
 * @param  : high:
 * @return : int32:
 */
int32 PID_Limiter(int32 amt, int32 low, int32 high) {
    return ((amt) < (low) ? (low) : ((amt) > (high) ? (high) : (amt)));
}

/**
 * @brief  : Limiting
 * @param  : amt:
 * @param  : low:
 * @param  : high:
 * @return : float:
 */
float PID_Limiter_float(float amt, float low, float high) {
    return ((amt) < (low) ? (low) : ((amt) > (high) ? (high) : (amt)));
}

/**
 * @brief  : PID Initialize
 * @param  : pid:
 */
void PID_Init(PID_t *pid) {
    pid->kP = 0.0f;
    pid->kI = 0.0f;
    pid->kD = 0.0f;

    pid->integral_limit = 0;
    pid->integrator = 0.0f;
    pid->last_error = 0;
    pid->last_derivative = 0;
}

/**
 * @brief  : PID Clear
 * @param  : pid:
 */
void PID_Clear(PID_t *pid) {
    pid->integrator = 0.0f;
    pid->last_error = 0;
    pid->last_derivative = 0;
}

/**
 * @brief  : PID λ��ʽ���������
 * @param  : pid:
 * @param  : actual:
 * @param  : set:
 * @return : int32:
 */
int32 PID_Position(PID_t *pid, int32 actual, int32 set) {
    int32 iError;    // ��ǰ���
    int32 Position;  // ���ó���ʵ�����

    /*���㵱ǰ���*/
    iError = set - actual;

    /* �ۻ���� - �˳�ģλ��ʽ��ʹ��PD */
    pid->integrator += pid->kI * iError;

    /* ����޷� */
    pid->integrator = PID_Limiter_float(pid->integrator, (float)(0 - pid->integral_limit), (float)(pid->integral_limit));

    /* �����ļ��� */
    Position = (int32)(pid->kP * iError + pid->integrator + pid->kD * (iError - pid->last_error));

    /* �����ϴ���� */
    pid->last_error = iError;

    return Position;
}

/**
 * @brief  : PID ����ʽ���������
 * @param  : pid:
 * @param  : actual:
 * @param  : set:
 * @return : int32:
 */
int32 PID_Increase(PID_t *pid, int32 actual, int32 set) {
    int32 iError;    // ��ǰ���
    int32 Increase;  // ���ó���ʵ�����

    /* ���㵱ǰ��� */
    iError = set - actual;

    /* �����ļ��� */
    Increase = (int32)(pid->kP * (iError - pid->last_error) + pid->kI * iError + pid->kD * (iError - 2 * pid->last_error + pid->last_derivative));

    /* ����ǰ����� */
    pid->last_derivative = pid->last_error;
    /* �����ϴ���� */
    pid->last_error = iError;

    return Increase;
}
