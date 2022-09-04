/*
 * ui_parm.h
 *
 *  Created on: 2021��8��1��
 *      Author: XinnZ, Landian, HBUT
 *  XinnZ's Blog: https://blog.xinnz.cn/
 */

#ifndef CODE_BSP_UI_PARM_H_
#define CODE_BSP_UI_PARM_H_

#define EEPROM_SECTOR   5       //>EEPROM����

typedef struct
{
        unsigned char CurrentParm;
        unsigned char LastFocus;
        unsigned char CurrentFocus;
} Parm_Focus_t;
extern Parm_Focus_t Parm_Focus;

extern unsigned char Flag_Save; //>���������־λ

void UI_Parm_Tasks(void);
void UI_Parm_Save(void);
void UI_Parm_Read(void);

#endif /* CODE_BSP_UI_PARM_H_ */
