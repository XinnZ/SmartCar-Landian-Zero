/*
 * UI.c
 *
 *  Created on: 2021��8��1��
 *      Author: XinnZ, Landian, HBUT
 *  XinnZ's Blog: https://blog.xinnz.cn/
 */

#include "headfile.h"
#include "IfxScuCcu.h"
#include "IfxCpu_cfg.h"

/*
 * @brief   : UI_Init �����ʼ��
 * @param   : None
 * @return  : None
 * @example : None
 * @author  : XinnZ
 */
void UI_Init(void)
{
    UI_Focus.CurrentUI      = ui_Main;  //>��ʼ������
    UI_Focus.LastFocus      = 0xFF;     //>��ʼ��һ�ν���δ֪
    UI_Focus.CurrentFocus   = ui_Main;  //>��ʼ��ǰ����������

    Parm_Focus.CurrentParm  = 0xFF;     //>��ʼѡ����δ֪
    Parm_Focus.LastFocus    = 0;        //>��ʼ�ϴ�ѡ����
    Parm_Focus.CurrentFocus = 0;        //>��ʼ��ǰѡ����
}

/*
 * @brief   : UI_Main_Btn �����滭�˵�
 * @param   : ui      : ��������
 *            status  : 0: Normal - 1: Focus
 * @return  : None
 * @example : None
 * @author  : XinnZ
 */
void UI_Main_Btn(UI_e ui, uint8 status)
{
    switch (ui)
    {
    case ui_Main:
        break;
    case ui_Motor:
        IPS114_String(status * 8 + 35, 4 * 16, " >  Motor Control   ", Color_WHITE, Color_BLACK);
        break;
    case ui_MT9V034:
        IPS114_String(status * 8 + 35, 5 * 16, " >  MT9V034 Looking ", Color_WHITE, Color_BLACK);
        break;
    case ui_Modules:
        IPS114_String(status * 8 + 35, 6 * 16, " >  Modules Center  ", Color_WHITE, Color_BLACK);
        break;
    case ui_RUN:
        IPS114_String(status * 8 + 35, 7 * 16, " >  Run! Let's go!  ", Color_WHITE, Color_BLACK);
        break;
    }
}
/*
 * @brief   : UI_Main ������
 * @param   : None
 * @return  : None
 * @example : None
 * @author  : XinnZ
 */
void UI_Main(void)
{
    //char TEMP_SHOW[30] = {0};   //>�Դ�
    uint8 TEMP_DATAS = 0;

    IPS114_Clear(Color_BLACK);
    IPS114_String(12, 24, "HBUT  Beacon.LD ZERO.  2021", Color_PURPLE, Color_BLACK);
    IPS114_String(12, 40, " -  Landian LXZ ZHT HJQ  - ", Color_PURPLE, Color_BLACK);

    for (TEMP_DATAS = 1; TEMP_DATAS <= ui_RUN; TEMP_DATAS++)
        UI_Main_Btn(TEMP_DATAS, 0);

    while (UI_Focus.CurrentUI == ui_Main)
    {
        //sprintf(TEMP_SHOW, "0x%x", Key);
        //IPS114_String(0, 0, TEMP_SHOW, Color_WHITE, Color_BLACK);

        if (UI_Focus.CurrentFocus != UI_Focus.LastFocus)
        {
            UI_Main_Btn(UI_Focus.LastFocus, 0);
            UI_Main_Btn(UI_Focus.CurrentFocus, 1);
            UI_Focus.LastFocus = UI_Focus.CurrentFocus;
        }

        UI_Charge();    //>��ʾ����������Ϣ
    }
}

/*
 * @brief   : UI_Motor ������Խ���
 * @param   : None
 * @return  : None
 * @example : None
 * @author  : XinnZ
 */
void UI_Motor(void)
{
    char TEMP_SHOW[30] = {0};   //>�Դ�
    uint8 Var_tX=0, Var_tY=0,   //>�ű����������
          Flag_tFinded = 0;     //>�ű���ҵ���־λ

    IPS114_Clear(Color_BLACK);
    IPS114_String(40, 16, "Motor Control", Color_PURPLE, Color_BLACK);

    while (UI_Focus.CurrentUI == ui_Motor)
    {
        while (Parm_Focus.CurrentParm == 3)     //>����Ѱ��
        {
            if (IfxCpu_acquireMutex(&g_mutexLockIMAGE)) //>ͼ������
            {
                Var_tX = Looking.SoftX;         //>Ŀ��ƫ�� ��ʹ���
                Var_tY = Looking.SoftY;         //>Ŀ����� ��ʹ���
                Flag_tFinded = Looking.EUREKA;

                IPS114_Draw_Rectangle(0, 0, IMAGE_W + 1, IMAGE_H + 1, Color_RED);   //>��ͼ���
                if (Mode_Looking)
                    IPS114_IMAGE032_BIN(MT9V034.BIN[0], 0);         //>��ֵ����ʾ
                else
                    IPS114_IMAGE032_CON(CONV.IMAGE[0], CONV.count); //>��ͨ����ʾ

                if (Flag_tFinded)
                {
                    IPS114_Draw_Part(1 + Looking.X, 1, 1 + Looking.X, IMAGE_H, Color_RED);    //>��׼��
                    IPS114_Draw_Part(1, 1 + Looking.Y, IMAGE_W, 1 + Looking.Y,  Color_RED);
                    IPS114_Draw_Rectangle(Looking.sX - 1, Looking.sY - 1, Looking.eX + 1, Looking.eY + 1, Color_RED);
                }

                IfxCpu_releaseMutex(&g_mutexLockIMAGE);
            }

            if (IfxCpu_acquireMutex(&g_mutexLockMOTOR)) //>������ƻ���
            {
                if (Flag_tFinded)
                {
                    Motor.CurrentX = Var_tX;    //>��ǰƫ��(��ƫ��)
                    Motor.CurrentY = Var_tY;    //>��ǰλ��(��ƫ��)
                }
                else
                {
                    Motor.CurrentX = IMAGE_MIDDLE;
                    Motor.CurrentY = IMAGE_H;
                }

                IfxCpu_releaseMutex(&g_mutexLockMOTOR);
            }
            systick_delay_ms(0.5);
        }

        Motor.CurrentX = IMAGE_W / 2;
        Motor.TargetV  = Motor.Velocity;

        sprintf(TEMP_SHOW, "%.2fm/s ", Motor.Speed);
        IPS114_String(175, 1 * 16, TEMP_SHOW, Color_WHITE, Color_BLACK);    //>��ʵ�ٶ�

        //VOFA_Send(Motor.Speed, 0);

        sprintf(TEMP_SHOW, " (%05ld)  Encoder  (%05ld) ", (Motor.Encoder_R * 10), (Motor.Encoder_L * 10));
        IPS114_String(12, 2 * 16, TEMP_SHOW, Color_WHITE, Color_BLACK);     //>������ֵ

        sprintf(TEMP_SHOW, " (%05ld)    PWM    (%05ld) ", Motor.PWM_R, Motor.PWM_L);
        IPS114_String(12, 3 * 16, TEMP_SHOW, Color_WHITE, Color_BLACK);     //>���PWM

        sprintf(TEMP_SHOW, " (%05ld)   W - V    %5ld  ", Motor.Turn, Motor.Velocity);
        IPS114_String(12, 4 * 16, TEMP_SHOW, Color_WHITE, Color_BLACK);     //>Ŀ���ٶ�

        /* ���ٶ��ڻ� */
        /*sprintf(TEMP_SHOW, " P:  %2.2f ", PID_Omega.kP);
        IPS114_String(0, 5 * 16, TEMP_SHOW, Color_WHITE, Color_BLACK);      //>���ٶ��ڻ�PID-P
        sprintf(TEMP_SHOW, " I:    -   ");
        IPS114_String(0, 6 * 16, TEMP_SHOW, Color_WHITE, Color_BLACK);      //>���ٶ��ڻ�PID-I  ��PD����
        sprintf(TEMP_SHOW, " D:  %2.2f ", PID_Omega.kD);
        IPS114_String(0, 7 * 16, TEMP_SHOW, Color_WHITE, Color_BLACK);      //>���ٶ��ڻ�PID-D
*/
        /* λ�û� */
        sprintf(TEMP_SHOW, " P:  %2.2f ", PID_Location.kP);
        IPS114_String(0, 5 * 16, TEMP_SHOW, Color_WHITE, Color_BLACK);      //>λ�û�PID-P
        sprintf(TEMP_SHOW, " I:    -   ");
        IPS114_String(0, 6 * 16, TEMP_SHOW, Color_WHITE, Color_BLACK);      //>λ�û�PID-I  ��PD����
        sprintf(TEMP_SHOW, " D:  %2.2f ", PID_Location.kD);
        IPS114_String(0, 7 * 16, TEMP_SHOW, Color_WHITE, Color_BLACK);      //>λ�û�PID-D

        /* �Ƕ��⻷ */
        sprintf(TEMP_SHOW, " %2.2f ", PID_Angle.kP);
        IPS114_String(110, 5 * 16, TEMP_SHOW, Color_WHITE, Color_BLACK);    //>�Ƕ��⻷PID-P
        sprintf(TEMP_SHOW, "   -   ");
        IPS114_String(110, 6 * 16, TEMP_SHOW, Color_WHITE, Color_BLACK);    //>�Ƕ��⻷PID-I  ��PD����
        sprintf(TEMP_SHOW, " %2.2f ", PID_Angle.kD);
        IPS114_String(110, 7 * 16, TEMP_SHOW, Color_WHITE, Color_BLACK);    //>�Ƕ��⻷PID-D

        /* �ٶȻ� */
        sprintf(TEMP_SHOW, " %2.2f ", PID_Velocity_R.kP);
        IPS114_String(180, 5 * 16, TEMP_SHOW, Color_WHITE, Color_BLACK);    //>�ٶȻ�PID-P
        sprintf(TEMP_SHOW, " %2.2f ", PID_Velocity_R.kI);
        IPS114_String(180, 6 * 16, TEMP_SHOW, Color_WHITE, Color_BLACK);    //>�ٶȻ�PID-I
        sprintf(TEMP_SHOW, " %2.2f ", PID_Velocity_R.kD);
        IPS114_String(180, 7 * 16, TEMP_SHOW, Color_WHITE, Color_BLACK);    //>�ٶȻ�PID-D


        IPS114_String(0,   1 * 16, Parm_Focus.CurrentFocus ==  0 ? "<" : " ", Color_WHITE, Color_BLACK);
        IPS114_String(0,   2 * 16, Parm_Focus.CurrentFocus ==  1 ? "-" : " ", Color_WHITE, Color_BLACK);
        IPS114_String(0,   3 * 16, Parm_Focus.CurrentFocus ==  2 ? "^" : " ", Color_WHITE, Color_BLACK);
        IPS114_String(0,   4 * 16, Parm_Focus.CurrentFocus ==  3 ? ">" : " ", Color_GREEN, Color_BLACK);
        IPS114_String(231, 4 * 16, Parm_Focus.CurrentFocus ==  4 ? "<" : " ", Color_WHITE, Color_BLACK);
        IPS114_String(88,  5 * 16, Parm_Focus.CurrentFocus ==  5 ? "<" : " ", Color_WHITE, Color_BLACK);
        IPS114_String(88,  7 * 16, Parm_Focus.CurrentFocus ==  6 ? "<" : " ", Color_WHITE, Color_BLACK);
        IPS114_String(166, 5 * 16, Parm_Focus.CurrentFocus ==  7 ? "<" : " ", Color_WHITE, Color_BLACK);
        IPS114_String(166, 7 * 16, Parm_Focus.CurrentFocus ==  8 ? "<" : " ", Color_WHITE, Color_BLACK);
        IPS114_String(231, 5 * 16, Parm_Focus.CurrentFocus ==  9 ? "<" : " ", Color_WHITE, Color_BLACK);
        IPS114_String(231, 6 * 16, Parm_Focus.CurrentFocus == 10 ? "<" : " ", Color_WHITE, Color_BLACK);
        IPS114_String(231, 7 * 16, Parm_Focus.CurrentFocus == 11 ? "<" : " ", Color_WHITE, Color_BLACK);

        UI_Charge();    //>��ʾ����������Ϣ
    }
}

/*
 * @brief   : UI_Charge ����������ֵ��ʾ
 * @param   : None
 * @return  : None
 * @example : None
 * @author  : XinnZ
 */
void UI_Charge(void)
{
    char TEMP_SHOW[30] = {0};   //>�Դ�

    //>����
    if (Charge.V8417 < 0.7f)    //>ORGV-0.9=1.65-0.9=0.75; ��ֹ����ŵ��յ�· 0.6V5A ��һ�������⣬��ֹͣ���
        sprintf(TEMP_SHOW, "I: error  ");
    else
        sprintf(TEMP_SHOW, "I: %2.1fA ", Charge.A);
    IPS114_String(80, 0, TEMP_SHOW, Color_PINK, Color_BLACK);

    //>��ѹ
    sprintf(TEMP_SHOW, "U: %2.1fV ", Charge.V);
    IPS114_String(0, 0, TEMP_SHOW, Color_PINK, Charge.V > LIMIT_V_MAX ? Color_GREEN : Color_BLACK);

    //>����
    sprintf(TEMP_SHOW, "P: %2.1fW", Charge.P);
    IPS114_String(160, 0, TEMP_SHOW, Color_PINK, Color_BLACK);
}

/*
 * @brief   : UI_MT9V034  ���ű�~~~
 * @param   : None
 * @return  : None
 * @example : None
 * @author  : XinnZ
 */
void UI_MT9V034(void)
{
    char TEMP_SHOW[21] = {0};   //>�Դ�
    //uint32 TEMP_DATAS  =  0;    //>��ʱ

    IPS114_Clear(Color_BLACK);

    while (UI_Focus.CurrentUI == ui_MT9V034)
    {
        if (IfxCpu_acquireMutex(&g_mutexLockIMAGE)) //>ͼ������
        {
            //IPS114_IMAGE032(MT9V034.IMAGE[0], IMAGE_W, IMAGE_H);
            //IPS114_IMAGE032(MT9V034.UNDST[0], IMAGE_W, IMAGE_H);
            //MT9V034_Send_IMG(MT9V034.IMAGE[0], IMAGE_W, IMAGE_H);

            IPS114_Draw_Rectangle(0, 0, IMAGE_W + 1, IMAGE_H + 1, Color_GREEN); //>��ͼ���

            if (Mode_Looking)
                IPS114_IMAGE032_BIN(MT9V034.BIN[0], 0);         //>��ֵ����ʾ
            else
            {
                IPS114_IMAGE032_CON(CONV.IMAGE[0], CONV.count); //>��ͨ����ʾ
                //IPS114_IMAGE032_CON(CONV.EDGES[0], 1);          //>��ͨ��߽���ʾ
            }

            if (Looking.EUREKA) //>�ҵ���׼��
            {
                IPS114_Draw_Part(1 + Looking.X, 1, 1 + Looking.X, IMAGE_H, Color_GREEN);
                IPS114_Draw_Part(1, 1 + Looking.Y, IMAGE_W, 1 + Looking.Y, Color_GREEN);
                IPS114_Draw_Rectangle(Looking.sX - 1, Looking.sY - 1, Looking.eX + 1, Looking.eY + 1, Color_GREEN);
            }

            sprintf(TEMP_SHOW, "ADD:%03d", MT9V034.threshold_offset);
            IPS114_String(178, 0 * 16, TEMP_SHOW, Color_WHITE, Color_BLACK);    //>��ֵ����ֵ����
            sprintf(TEMP_SHOW, "THR:%03d", MT9V034.threshold);
            IPS114_String(178, 1 * 16, TEMP_SHOW, Color_WHITE, Color_BLACK);    //>��ֵ����ֵ
            sprintf(TEMP_SHOW, "TOT:%03d", CONV.count);
            IPS114_String(178, 2 * 16, TEMP_SHOW, Color_WHITE, Color_BLACK);    //>�ܵ���ͨ������
            sprintf(TEMP_SHOW, "COR:%03d", Looking.count);
            IPS114_String(178, 3 * 16, TEMP_SHOW, Color_WHITE, Color_BLACK);    //>������������ͨ�����

            sprintf(TEMP_SHOW, "X:%03d", Looking.X);
            IPS114_String(3, 102, TEMP_SHOW, Color_WHITE, Color_BLACK);     //>������ͨ����������X
            sprintf(TEMP_SHOW, "Y:%03d", Looking.Y);
            IPS114_String(3, 118, TEMP_SHOW, Color_WHITE, Color_BLACK);     //>������ͨ����������Y
            sprintf(TEMP_SHOW, "sX:%03d", Looking.sX);
            IPS114_String(52, 102, TEMP_SHOW, Color_WHITE, Color_BLACK);    //>������ͨ����������sX
            sprintf(TEMP_SHOW, "sY:%03d", Looking.sY);
            IPS114_String(52, 118, TEMP_SHOW, Color_WHITE, Color_BLACK);    //>������ͨ����������sY
            sprintf(TEMP_SHOW, "eX:%03d", Looking.eX);
            IPS114_String(110, 102, TEMP_SHOW, Color_WHITE, Color_BLACK);   //>������ͨ����������eX
            sprintf(TEMP_SHOW, "eY:%03d", Looking.eY);
            IPS114_String(110, 118, TEMP_SHOW, Color_WHITE, Color_BLACK);   //>������ͨ����������eY

            sprintf(TEMP_SHOW, "R:%.2f ", Looking.cRound);
            IPS114_String(178, 5 * 16 + 6, TEMP_SHOW, Color_WHITE, Color_BLACK);    //>������ͨ��Բ�ζ�
            sprintf(TEMP_SHOW, "S:%04d", Looking.cArea);
            IPS114_String(178, 6 * 16 + 6, TEMP_SHOW, Color_WHITE, Color_BLACK);    //>������ͨ�����
            sprintf(TEMP_SHOW, "C:%04d", Looking.cEdge);
            IPS114_String(178, 7 * 16 + 6, TEMP_SHOW, Color_WHITE, Color_BLACK);    //>������ͨ��߳�

            IfxCpu_releaseMutex(&g_mutexLockIMAGE);
        }
        systick_delay_ms(0.5);
    }
}

/*
 * @brief   : UI_Modules ģ����Խ���
 * @param   : None
 * @return  : None
 * @example : None
 * @author  : XinnZ
 */
void UI_Modules(void)
{
    char TEMP_SHOW[21] = {0};   //>�Դ�
    //uint16 TEMP_DATAS = 0;      //>��ʱ����

    IPS114_Clear(Color_BLACK);
    IPS114_String(72, 16, "What's this?", Color_PINK, Color_BLACK);

    while (UI_Focus.CurrentUI == ui_Modules)
    {
        UI_Charge();    //>��ʾ����������Ϣ

        //>������
        sprintf(TEMP_SHOW, "MAX: %.1fV ", LIMIT_V_MAX);
        IPS114_String(0, 2 * 16, TEMP_SHOW, Color_WHITE, Color_BLACK);
        sprintf(TEMP_SHOW, "MIN: %.1fV ", LIMIT_V_MIN);
        IPS114_String(0, 3 * 16, TEMP_SHOW, Color_WHITE, Color_BLACK);

        IPS114_String(0,  1 * 16, Parm_Focus.CurrentFocus ==  0 ? "<" : " ", Color_WHITE, Color_BLACK);
        IPS114_String(96, 2 * 16, Parm_Focus.CurrentFocus ==  1 ? "<" : " ", Color_WHITE, Color_BLACK);
        IPS114_String(96, 3 * 16, Parm_Focus.CurrentFocus ==  2 ? "<" : " ", Color_WHITE, Color_BLACK);

        //ICM42605_TemperatureData();
        //ICM42605_AccData();
        ICM42605_GyroData();

        //VOFA_Send(ICM.GYRO.X, ICM.GYRO.Y, ICM.GYRO.Z);

        //>������
        sprintf(TEMP_SHOW, "GYRO X: %.1f  ", ICM.GYRO.X);
        IPS114_String(0, 5 * 16, TEMP_SHOW, Color_WHITE, Color_BLACK);
        sprintf(TEMP_SHOW, "GYRO Y: %.1f  ", ICM.GYRO.Y);
        IPS114_String(0, 6 * 16, TEMP_SHOW, Color_WHITE, Color_BLACK);
        sprintf(TEMP_SHOW, "GYRO Z: %.1f  ", ICM.GYRO.Z);
        IPS114_String(0, 7 * 16, TEMP_SHOW, Color_WHITE, Color_BLACK);

        //>���ٶ�
        /*sprintf(TEMP_SHOW, "-  %.2f  ", ICM.ACCEL.X);
        IPS114_String(100, 5 * 16, TEMP_SHOW, Color_WHITE, Color_BLACK);
        sprintf(TEMP_SHOW, "-  %.2f  ", ICM.ACCEL.Y);
        IPS114_String(100, 6 * 16, TEMP_SHOW, Color_WHITE, Color_BLACK);
        sprintf(TEMP_SHOW, "-  %.2f  ", ICM.ACCEL.Z);
        IPS114_String(100, 7 * 16, TEMP_SHOW, Color_WHITE, Color_BLACK);
         */

        //>�¶�
        /*sprintf(TEMP_SHOW, "temp: %3.2f  ", ICM.TEMP);
        //IPS114_String(0, 5 * 16, TEMP_SHOW, Color_WHITE, Color_BLACK);
         */

        systick_delay_ms(1);
    }
}
