/*
 * RUN.c
 *
 *  Created on: 2021��8��1��
 *      Author: XinnZ, Landian, HBUT
 *  XinnZ's Blog: https://blog.xinnz.cn/
 */

#include "headfile.h"

Flag_t Flag = {0};
Stage_t Stage[3] = {
    // TP    UT   CP   UC  AP  AC    OP
    {15.0f, 58, 7.0f, 92, 65, 10, 0.33f},
    {17.5f, 66, 7.0f, 92, 70, 10, 0.43f},
    {20.0f, 70, 7.0f, 92, 75, 10, 0.50f}};

/**
 * @brief  : Just an initialization
 */
void RUN_Init(void) {
    Flag.Mode_CAP = (_Bool)Mode[1][0];

    // ���뿪��ѡ���ٶ�
    for (uint8 n = 1; n < 4; n++) {
        if (Mode[1][n])
            Flag.IndexV = n - 1;
    }

    Flag.FirstCharge = 1;
}

/**
 * @brief  : RUN_GoGoGo Go Go Go������
 */
void RUN_GoGoGo(void) {
    uint8 Var_aX = 0, Var_aY = 0;     // �ű������ԭʼ����
    uint32 Timing, ChargeTiming = 0;  // �ܼ�ʱ, ��������ʱ

    LED0_ON;                                 // ���ָʾ
    IPS114_BL(0);                            // ����Ļ
    Motor.ENABLE = 0;                        // �ص��
    pit_disable_interrupt(CCU6_1, PIT_CH1);  // ���������

    Timing = Motor.TIME;

    while (UI_Focus.CurrentUI == ui_RUN) {
        if (IfxCpu_acquireMutex(&g_mutexLockIMAGE))  // ͼ������
        {
            Flag.FindTarget = Looking.EUREKA;

            // Var_sX = Looking.SoftX; // Ŀ������� ��ʹ���
            // Var_sY = Looking.SoftY; // Ŀ�������� ��ʹ���
            Var_aX = Looking.X;  // Ŀ��ԭʼ������
            Var_aY = Looking.Y;  // Ŀ��ԭʼ������

            IfxCpu_releaseMutex(&g_mutexLockIMAGE);
        }

        if (IfxCpu_acquireMutex(&g_mutexLockMOTOR))  // ������ƻ���
        {
            PID_Omega.kP = Stage[Flag.IndexV].OmegaP;

            if (Flag.Charged)  // ������
            {
                if (Flag.FindTarget)  // �ҵ�Ŀ��
                {
                    if (!Flag.ToCharge)  // ������ ����Ŀ��
                    {
                        Motor.CurrentX = Var_aX + Stage[Flag.IndexV].AngleCOR;  // ��ǰƫ�� ( + ���� )
                        Motor.CurrentY = Var_aY;                                // ��ǰ����

                        PID_Location.kP = Stage[Flag.IndexV].TatgetP;

                        if (Var_aY >= 60)  // ����
                            PID_Location.kP /= 2;
                        if (Var_aY >= Stage[Flag.IndexV].UphillT)  // �������ٶ�
                            Motor.CurrentY = Stage[Flag.IndexV].UphillT;

                        if ((Var_aY > (IMAGE_H / 2)) && (abs(Var_aX - IMAGE_MIDDLE) > 15))
                            PID_Angle.kP = Stage[Flag.IndexV].AngleP + 20;
                        else
                            PID_Angle.kP = Stage[Flag.IndexV].AngleP;
                    } else  // ��Ҫ��� ׼�����
                    {
                        Motor.CurrentX = Var_aX;  // ��ǰƫ��
                        Motor.CurrentY = Var_aY;  // ��ǰ����
                        Flag.ChargeReady = 1;     // ���׼��

                        PID_Angle.kP = Stage[Flag.IndexV].AngleP + 10;
                        PID_Location.kP = Stage[Flag.IndexV].ChatgeP;
                        if (Var_aY >= Stage[Flag.IndexV].UphillC)  // �������ٶ�
                            Motor.CurrentY = Stage[Flag.IndexV].UphillC;
                    }
                } else  // δ�ҵ� ��ת�ҵ�
                {
                    if (Flag.ToCharge && Flag.ChargeReady)  // ��Ҫͣ�����
                    {
                        Motor.CurrentX = IMAGE_MIDDLE;
                        Motor.CurrentY = Stage[Flag.IndexV].UphillC;  // ��λ�û�ƫ�� �����ϵ�
                        PID_Location.kP = Stage[Flag.IndexV].ChatgeP;
                        Flag.Charged = 0;  // ��ʼ���
                        Flag.ChargeReady = 0;
                        ChargeTiming = Motor.TIME;
                    } else {
                        Motor.CurrentX = 0;
                        Motor.CurrentY = Stage[Flag.IndexV].UphillT;
                        ;
                        PID_Angle.kP = Stage[Flag.IndexV].AngleP + 10;
                    }
                }
            } else  // ���δ���
            {
                Motor.CurrentX = IMAGE_MIDDLE;
                Motor.CurrentY = IMAGE_H;
            }

            if ((Charge.V > (Flag.Mode_CAP ? 9.5f : 11.0f)) && !Flag.FirstCharge)
                goto Charging_Complete;  // ���״γ��
            if ((Charge.V > (Flag.Mode_CAP ? 6.0f : 8.5f)) && !Flag.FirstCharge && (Motor.TIME > 1000 * 160))
                goto Charging_Complete;  // ���״γ����ʱ�������ְ� �ӿ��ٶ�
            if (Flag.ToCharge && !Flag.Charged && !Flag.FirstCharge) {
                if (Charge.A > 4.8f)  // ������ֵ
                    Motor.CurrentY = IMAGE_H;
                else
                    Motor.CurrentY = Stage[Flag.IndexV].UphillC;
                if (Flag.FindTarget)  // ������Ŀ��
                    goto Charging_Complete;
                if (Charge.A < 4.8f && ((Motor.TIME - ChargeTiming) > 3500))  // ����3���δ�ڳ����������
                    goto Charging_Complete;
            }

            if (Charge.V > (Flag.Mode_CAP ? 7.2f : 11.0f))  // LIMIT_V_MAX)) && Flag.FirstCharge) // ������
            {
            Charging_Complete:
                LED0_OFF;              // �رճ��ָʾ
                Motor_STBY_H;          // ���������
                Motor.ENABLE = 1;      // �����
                Flag.ToCharge = 0;     // ������
                Flag.Charged = 1;      // ������
                Flag.FirstCharge = 0;  // ��һ�γ�����
            }
            if (Charge.V < (Flag.Mode_CAP ? 3.5f : 4.0f) && (Motor.TIME - Timing) <= 1000 * 170)  // LIMIT_V_MIN ��ѹ������ʱ���ԣ
            {
                LED0_ON;            // ���ָʾ
                Flag.ToCharge = 1;  // ��Ҫ���
            }

            if ((Motor.TIME - Timing) >= 1000 * 180)  // �������ѵ�
            {
                if (!Flag.FindTarget) {
                    UI_Focus.CurrentUI = ui_Main;
                    LED0_ON;
                    LED1_ON;
                    LED2_ON;
                    LED3_ON;
                    Motor.CurrentX = IMAGE_MIDDLE;
                    Motor.CurrentY = IMAGE_H;
                }
            }

            IfxCpu_releaseMutex(&g_mutexLockMOTOR);
        }
        systick_delay_ms(1);
    }
}
