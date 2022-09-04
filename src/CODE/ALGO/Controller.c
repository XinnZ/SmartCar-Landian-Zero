/*
 * Controller.c
 *
 *  Created on: 2021��8��1��
 *      Author: XinnZ, Landian, HBUT
 *  XinnZ's Blog: https://blog.xinnz.cn/
 */

#include "Controller.h"

/**
 * @brief  : Angle and Omega(��) Control
 */
void Car_Looking(void) {
    /* ���ݻ�ȡ - �����ǽ��ٶ� */
    ICM42605_GyroData();

    /* �㷨���� - �Ƕ��⻷ */
    Motor.Turn = PID_Position(&PID_Angle, Motor.CurrentX, (int32)(IMAGE_MIDDLE));

    /* �㷨���� - ���ٶ��ڻ� */
    Motor.Turn = PID_Position(&PID_Omega, (int32)(ICM.GYRO.Z * 10), Motor.Turn);
    Motor.Turn = PID_Limiter(Motor.Turn, -6000, 6000);
}

/**
 * @brief  : Velocity Control
 */
void Car_Going(void) {
    /* ���ݻ�ȡ - ������ֵ */
    Motor.Encoder_R = gpt12_get(Motor_R_T);
    gpt12_clear(Motor_R_T);
    Motor.Encoder_L = -gpt12_get(Motor_L_T);
    gpt12_clear(Motor_L_T);

    /* ���ݻ�ȡ - ��ʵ�ٶ� */
    Motor.Speed = (Motor.Encoder_R + Motor.Encoder_L) * 0.016349f;
    // ((((Motor.Encoder_R + Motor.Encoder_L) / 2) * 30 * 3.1415926 * 0.0675) / 38 / 1024) / 0.005;
    // 30��d / 38 / 1024 = 1(һ�������Ӧ��·��)

    /* �㷨���� - λ���⻷ */
    Motor.TargetV = PID_Position(&PID_Location, Motor.CurrentY, (int32)(IMAGE_H));

    /* �㷨���� - �ٶ��ڻ� */
    Motor.PWM_R += PID_Increase(&PID_Velocity_R, Motor.Encoder_R * 10, Motor.TargetV + Motor.Turn);
    Motor.PWM_L += PID_Increase(&PID_Velocity_L, Motor.Encoder_L * 10, Motor.TargetV - Motor.Turn);

    /* �ٶ��޷� - ����ռ�ձ� */
    Motor.PWM_R = PID_Limiter(Motor.PWM_R, -Motor_MAX_LIMIT, Motor_MAX_LIMIT);
    Motor.PWM_L = PID_Limiter(Motor.PWM_L, -Motor_MAX_LIMIT, Motor_MAX_LIMIT);

    /* �ٶȿ��� */
    Motor_Speed(Motor.PWM_R, Motor.PWM_L);
}
