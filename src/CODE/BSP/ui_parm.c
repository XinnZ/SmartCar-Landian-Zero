/*
 * ui_parm.c
 *
 *  Created on: 2021��8��1��
 *      Author: XinnZ, Landian, HBUT
 *  XinnZ's Blog: https://blog.xinnz.cn/
 */

#include "headfile.h"

uint8 Flag_Save = 0;

/*
 * @brief   : UI_Parm_Tasks
 * @param   : None
 * @return  : None
 * @example : None
 * @author  : XinnZ
 */
void UI_Parm_Tasks(void)
{
    switch (UI_Focus.CurrentUI)
    {
    /* ****************************  ���˵�����  **************************** */
    case ui_Main:
    {
        switch (Key)
        {
        case Key_Left:
        case Key_Up:
        {
            if (UI_Focus.CurrentFocus <= 1)
                UI_Focus.CurrentFocus = ui_RUN;
            else
                UI_Focus.CurrentFocus--;
        }
        break;
        case Key_Right:
        case Key_Press:
        {
            UI_Focus.CurrentUI = UI_Focus.CurrentFocus; //>ѡ��
        }
        break;
        case Key_Down:
        {
            if (UI_Focus.CurrentFocus >= ui_RUN)
                UI_Focus.CurrentFocus = 1;
            else
                UI_Focus.CurrentFocus++;
        }
        break;
        }
    }
    break;

    /* *************************  ������Ʋ˵�����  ************************* */
    case ui_Motor:
    {
        switch (Key)
        {
        case Key_Down:
        {
            if (Parm_Focus.CurrentFocus == Parm_Focus.CurrentParm)
            {
                Parm_Focus.CurrentParm = 0xFF; //>ȡ��ѡ��
            }
            Parm_Focus.CurrentFocus++;         //>���ƹ��
            if (Parm_Focus.CurrentFocus == 12) //>������Χ
                Parm_Focus.CurrentFocus = 0;   //>�ص���һλ
        }
        break;
        case Key_Right:
        {
            switch (Parm_Focus.CurrentFocus)
            {
            case 4:
                Motor.Velocity  += Parm_Focus.CurrentFocus == Parm_Focus.CurrentParm ?  100  :   50;
                break;
            case 5:
                PID_Location.kP += Parm_Focus.CurrentFocus == Parm_Focus.CurrentParm ? 1.0f :  0.1f;
                break;
            case 6:
                PID_Location.kD += Parm_Focus.CurrentFocus == Parm_Focus.CurrentParm ? 0.1f : 0.01f;
                break;
            case 7:
                PID_Angle.kP    += Parm_Focus.CurrentFocus == Parm_Focus.CurrentParm ? 1.0f :  0.1f;
                break;
            case 8:
                PID_Angle.kD    += Parm_Focus.CurrentFocus == Parm_Focus.CurrentParm ? 0.1f : 0.01f;
                break;
            case 9:
                PID_Velocity_R.kP = PID_Velocity_L.kP += Parm_Focus.CurrentFocus == Parm_Focus.CurrentParm ? 1.0f :  0.1f;
                break;
            case 10:
                PID_Velocity_R.kI = PID_Velocity_L.kI += Parm_Focus.CurrentFocus == Parm_Focus.CurrentParm ? 0.1f : 0.01f;
                break;
            case 11:
                PID_Velocity_R.kD = PID_Velocity_L.kD += Parm_Focus.CurrentFocus == Parm_Focus.CurrentParm ? 0.1f : 0.01f;
                break;
            }
        }
        break;
        case Key_Press:
        {
            if (Parm_Focus.CurrentFocus == 0)       //>�ص�������
            {
                Motor.ENABLE = 0;
                Motor_STBY_L;
                Motor_Stop();
                UI_Init();
            }
            else if (Parm_Focus.CurrentFocus == 1)  //�������
            {
                Flag_Save = 1;
            }
            else if (Parm_Focus.CurrentFocus == 2)  //>���ʹ��
            {
                Motor.ENABLE = Motor.ENABLE ? false : true;
                if (Motor.ENABLE)
                {
                    Motor_STBY_H;
                }
                else
                {
                    Motor_STBY_L;
                    Motor_Stop();
                }
            }
            else if (Parm_Focus.CurrentParm != Parm_Focus.CurrentFocus)
                Parm_Focus.CurrentParm = Parm_Focus.CurrentFocus; //>ѡ�е�ǰ���
            else
                Parm_Focus.CurrentParm = 0xFF;  //>ȡ��ѡ��
        }
        break;
        case Key_Up:
        {
            if (Parm_Focus.CurrentFocus == Parm_Focus.CurrentParm)
            {
                Parm_Focus.CurrentParm = 0xFF;  //>ȡ��ѡ��
            }
            Parm_Focus.CurrentFocus--;          //>���ƹ��
            if (Parm_Focus.CurrentFocus > 12)   //>������Χ
                Parm_Focus.CurrentFocus = 11;   //>�ص����һλ
        }
        break;
        case Key_Left:
        {
            switch (Parm_Focus.CurrentFocus)
            {
            case 4:
                Motor.Velocity  -= Parm_Focus.CurrentFocus == Parm_Focus.CurrentParm ?  100 :    50;
                break;
            case 5:
                PID_Location.kP -= Parm_Focus.CurrentFocus == Parm_Focus.CurrentParm ? 1.0f :  0.1f;
                break;
            case 6:
                PID_Location.kD -= Parm_Focus.CurrentFocus == Parm_Focus.CurrentParm ? 0.1f : 0.01f;
                break;
            case 7:
                PID_Angle.kP    -= Parm_Focus.CurrentFocus == Parm_Focus.CurrentParm ? 1.0f :  0.1f;
                break;
            case 8:
                PID_Angle.kD    -= Parm_Focus.CurrentFocus == Parm_Focus.CurrentParm ? 0.1f : 0.01f;
                break;
            case 9:
                PID_Velocity_R.kP = PID_Velocity_L.kP -= Parm_Focus.CurrentFocus == Parm_Focus.CurrentParm ? 1.0f :  0.1f;
                break;
            case 10:
                PID_Velocity_R.kI = PID_Velocity_L.kI -= Parm_Focus.CurrentFocus == Parm_Focus.CurrentParm ? 0.1f : 0.01f;
                break;
            case 11:
                PID_Velocity_R.kD = PID_Velocity_L.kD -= Parm_Focus.CurrentFocus == Parm_Focus.CurrentParm ? 0.1f : 0.01f;
                break;
            }
        }
        break;
        }
    }
    break;

    /* **************************  ����ͷ�˵�����  ************************** */
    case ui_MT9V034:
    {
        switch (Key)
        {
        case Key_Up:
        {
            MT9V034.threshold_offset += 10;
        }
        break;
        case Key_Right:
        {
            MT9V034.threshold_offset += 1;
        }
        break;
        case Key_Press:
        {
            Flag_Save = 1; //>��������
            UI_Init();
        }
        break;
        case Key_Down:
        {
            MT9V034.threshold_offset -= 10;
        }
        break;
        case Key_Left:
        {
            MT9V034.threshold_offset -= 1;
        }
        break;
        }
    }
    break;

    /* *************************  ����ģ��˵�����  ************************* */
    case ui_Modules:
    {
        switch (Key)
        {
        case Key_Down:
        {
            if (Parm_Focus.CurrentFocus == Parm_Focus.CurrentParm)
                Parm_Focus.CurrentParm = 0xFF;  //>ȡ��ѡ��
            Parm_Focus.CurrentFocus++;          //>���ƹ��
            if (Parm_Focus.CurrentFocus == 3)   //>������Χ
                Parm_Focus.CurrentFocus = 0;    //>�ص���һλ
        }
        break;
        case Key_Right:
        {
            switch (Parm_Focus.CurrentFocus)
            {
            case 1:
                LIMIT_V_MAX += Parm_Focus.CurrentFocus == Parm_Focus.CurrentParm ? 1.0f : 0.1f;
                break;
            case 2:
                LIMIT_V_MIN += Parm_Focus.CurrentFocus == Parm_Focus.CurrentParm ? 1.0f : 0.1f;
                break;
            }
        }
        break;
        case Key_Press:
        {
            if (Parm_Focus.CurrentFocus == 0)
            {
                Flag_Save = 1;
                UI_Init();  //>�ص����˵�
            }
            else if (Parm_Focus.CurrentParm != Parm_Focus.CurrentFocus)
                Parm_Focus.CurrentParm = Parm_Focus.CurrentFocus; //>ѡ�е�ǰ���
            else
                Parm_Focus.CurrentParm = 0xFF; //>ȡ��ѡ��
        }
        break;
        case Key_Up:
        {
            if (Parm_Focus.CurrentFocus == Parm_Focus.CurrentParm)
                Parm_Focus.CurrentParm = 0xFF; //>ȡ��ѡ��
            Parm_Focus.CurrentFocus--;         //>���ƹ��
            if (Parm_Focus.CurrentFocus > 3)   //>������Χ
                Parm_Focus.CurrentFocus = 2;   //>�ص����һλ
        }
        break;
        case Key_Left:
        {
            switch (Parm_Focus.CurrentFocus)
            {
            case 1:
                LIMIT_V_MAX -= Parm_Focus.CurrentFocus == Parm_Focus.CurrentParm ? 1.0f : 0.1f;
                break;
            case 2:
                LIMIT_V_MIN -= Parm_Focus.CurrentFocus == Parm_Focus.CurrentParm ? 1.0f : 0.1f;
                break;
            }
        }
        break;
        }
    }
    }
}

/*
 * @brief   : UI_Parm_Save
 * @param   : None
 * @return  : None
 * @example : None
 * @author  : XinnZ
 */
void UI_Parm_Save(void)
{
    uint32 index = 10;      //>��������
    uint32 write_buf = 0;   //>�����ݴ�
    uint32 SECTOR_buf = 0;  //>����

    disableInterrupts();    //>�ر����ж�

    Flag_Save = 0;          //>�����־����

    LED3_ON;                //>LED3 On

    //>��鵱ǰҳ�Ƿ������� �������������Ҫ������������
    for (; SECTOR_buf < 512; SECTOR_buf++)
    {
        if (flash_check(EEPROM_SECTOR, SECTOR_buf) == 1)
        {
            eeprom_erase_sector(EEPROM_SECTOR);
            break;
        }
    }

    //>��д������ݲ���32λʱ ���齫���ݷ���uint32 ���͵ı����� �ڽ��ñ�����Ϊ�������뵽eeprom_page_program����
    write_buf = Motor.Velocity;
    eeprom_page_program(EEPROM_SECTOR, index++, &write_buf);    //>������ֱ��Ŀ��ֵ

    write_buf = float_conversion_uint32(PID_Angle.kP);
    eeprom_page_program(EEPROM_SECTOR, index++, &write_buf);    //>���渡���� PID_Angle-P
    write_buf = float_conversion_uint32(PID_Angle.kD);
    eeprom_page_program(EEPROM_SECTOR, index++, &write_buf);    //>���渡���� PID_Angle-D

    write_buf = float_conversion_uint32(PID_Omega.kP);
    eeprom_page_program(EEPROM_SECTOR, index++, &write_buf);    //>���渡���� PID_Omega-P
    write_buf = float_conversion_uint32(PID_Omega.kD);
    eeprom_page_program(EEPROM_SECTOR, index++, &write_buf);    //>���渡���� PID_Omega-D

    write_buf = float_conversion_uint32(PID_Location.kP);
    eeprom_page_program(EEPROM_SECTOR, index++, &write_buf);    //>���渡���� PID_Location-P
    write_buf = float_conversion_uint32(PID_Location.kD);
    eeprom_page_program(EEPROM_SECTOR, index++, &write_buf);    //>���渡���� PID_Location-D

    write_buf = float_conversion_uint32(PID_Velocity_R.kP);
    eeprom_page_program(EEPROM_SECTOR, index++, &write_buf);    //>���渡���� PID_Velocity_R_L-P
    write_buf = float_conversion_uint32(PID_Velocity_R.kI);
    eeprom_page_program(EEPROM_SECTOR, index++, &write_buf);    //>���渡���� PID_Velocity_R_L-I
    write_buf = float_conversion_uint32(PID_Velocity_R.kD);
    eeprom_page_program(EEPROM_SECTOR, index++, &write_buf);    //>���渡���� PID_Velocity_R_L-D

    write_buf = (uint32)MT9V034.threshold_offset;
    eeprom_page_program(EEPROM_SECTOR, index++, &write_buf);    //>������ֵ����
    write_buf = (uint32)MT9V034.threshold;
    eeprom_page_program(EEPROM_SECTOR, index++, &write_buf);    //>������ֵ

    write_buf = float_conversion_uint32(LIMIT_V_MAX);
    eeprom_page_program(EEPROM_SECTOR, index++, &write_buf);    //>����������ѹֵ

    write_buf = float_conversion_uint32(LIMIT_V_MIN);
    eeprom_page_program(EEPROM_SECTOR, index,   &write_buf);    //>��������С��ѹֵ

    LED3_OFF;           //>LED Off
    enableInterrupts(); //>�������ж�
}

/*
 * @brief   : UI_Parm_Read
 * @param   : None
 * @return  : None
 * @example : None
 * @author  : XinnZ
 */
void UI_Parm_Read(void)
{
    uint8 index = 10;   //>��ȡ����

    Motor.Velocity = flash_read(EEPROM_SECTOR, index++, int32);     //>��ȡ���ֱ��Ŀ��ֵ

    PID_Angle.kP = flash_read(EEPROM_SECTOR, index++, float);       //>���Ƕ��⻷P
    PID_Angle.kD = flash_read(EEPROM_SECTOR, index++, float);       //>���Ƕ��⻷D
    PID_Angle.kI = 0.0f;                                            //>���Ƕ��⻷I

    PID_Omega.kP = flash_read(EEPROM_SECTOR, index++, float);       //>�����ٶ��ڻ�P
    PID_Omega.kD = flash_read(EEPROM_SECTOR, index++, float);       //>�����ٶ��ڻ�D
    PID_Omega.kI = 0.0f;                                            //>�����ٶ��ڻ�I

    PID_Location.kP = flash_read(EEPROM_SECTOR, index++, float);    //>��λ���⻷P
    PID_Location.kD = flash_read(EEPROM_SECTOR, index++, float);    //>��λ���⻷D
    PID_Location.kI = 0.0f;                                         //>��λ���⻷I

    PID_Velocity_L.kP = flash_read(EEPROM_SECTOR, index++, float);  //>���ٶȻ�L_P
    PID_Velocity_L.kI = flash_read(EEPROM_SECTOR, index++, float);  //>���ٶȻ�L_I
    PID_Velocity_L.kD = flash_read(EEPROM_SECTOR, index++, float);  //>���ٶȻ�L_D

    PID_Velocity_R.kP = PID_Velocity_L.kP;      //>���ٶȻ�R_P
    PID_Velocity_R.kI = PID_Velocity_L.kI;      //>���ٶȻ�R_I
    PID_Velocity_R.kD = PID_Velocity_L.kD;      //>���ٶȻ�R_D

    MT9V034.threshold_offset = flash_read(EEPROM_SECTOR, index++, uint8);   //>��ȡ��ֵ����
    MT9V034.threshold = flash_read(EEPROM_SECTOR, index++, uint8);          //>��ȡ��ֵ

    LIMIT_V_MAX = flash_read(EEPROM_SECTOR, index++, float);    //>���������ѹ
    LIMIT_V_MIN = flash_read(EEPROM_SECTOR, index,   float);    //>�������С��ѹ

    Mode[0][0] = !gpio_get(DIP4);   // MT9V034
    Mode[0][1] = !gpio_get(DIP5);   // Threshold
    Mode[0][2] = !gpio_get(DIP6);   // Looking
    Mode[0][3] = !gpio_get(DIP7);   // Run
    Mode[1][0] = !gpio_get(DIP0);
    Mode[1][1] = !gpio_get(DIP1);
    Mode[1][2] = !gpio_get(DIP2);
    Mode[1][3] = !gpio_get(DIP3);
}
