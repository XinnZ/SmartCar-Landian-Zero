/*
 * Looking.c
 *
 *  Created on: 2021��8��1��
 *      Author: XinnZ, Landian, HBUT
 *  XinnZ's Blog: https://blog.xinnz.cn/
 */

#include "Looking.h"

#include <math.h>
#include <stdlib.h>

#include "../APP/MT9V034.h"
#include "TwoPass.h"

/**
 * @brief  : Find the Beacon and determine the coordinates
 * @param  : look:
 * @param  : camera:
 * @param  : con:
 */
void Looking_Target(Looking_t *look, MT9V034_t camera, CON_t con) {
    uint8 x, y, cCount = 0;
    int32 index = 0;
    uint32 SumX = 0, SumY = 0;  // ������������
    CON_Node *SubCON = NULL;    // ����ͨ��
    CON_Node *FitCON = NULL;    // ��������������ͨ��

    if (Mode_Looking) {
        /* *************************  ���ж����б�  ************************* */
        /* *********** Զ�����ű�ƹ�߷ǳ�С  ʹ�ú������Զ��������ж� *********** */
        for (y = IMAGE_ignoreTop; y < IMAGE_H - IMAGE_ignoreBottom; y++) {
            for (x = 1; x < IMAGE_W; x++) {
                if (camera.BIN[y][x - 1] &&  // �����������ж�Ϊ�ű�
                    camera.BIN[y][x]) {      // && camera.BIN[y][x + 1]
                    SumX += x;
                    SumY += y;
                    index += 1;
                }
            }
        }

        if (index) {
            look->Y = (uint8)(SumY / index);
            look->X = (uint8)(SumX / index);
            look->EUREKA = 1;  // �����ű��
        } else {
            look->Y = 0;
            look->X = 0;
            look->EUREKA = 0;  // δ�����ű��
        }
    } else {
        /* *****************************  ��ͨ���б�  ***************************** */
        if (con.count) {
            SubCON = (CON_Node *)malloc(sizeof(CON_Node) * con.count);
            memset(SubCON, 0, sizeof(CON_Node) * con.count);

            for (y = IMAGE_ignoreTop; y < IMAGE_H - IMAGE_ignoreBottom; y++) {
                for (x = 0; x < IMAGE_W; x++) {
                    index = con.IMAGE[y][x] - 1;
                    if (index >= 0) {
                        if (SubCON[index].nsX == 0)
                            SubCON[index].nsX = x;  // Start X
                        if (SubCON[index].nsY == 0)
                            SubCON[index].nsY = y;  // Start Y

                        SubCON[index].nsX = x < SubCON[index].nsX ? x : SubCON[index].nsX;  // Start X
                        SubCON[index].nsY = y < SubCON[index].nsY ? y : SubCON[index].nsY;  // Start Y
                        SubCON[index].neX = x > SubCON[index].neX ? x : SubCON[index].neX;  // End X
                        SubCON[index].neY = y > SubCON[index].neY ? y : SubCON[index].neY;  // End Y
                        SubCON[index].nArea += 1;                                           // ���

                        if (x == 0 || y == 0)  // �߳�
                        {
                            CONV.EDGES[y][x] = (uint16)index + 1;
                            SubCON[index].nEdge += 1;
                        } else {
                            if (!(con.IMAGE[y + 1][x] && con.IMAGE[y - 1][x] &&
                                  con.IMAGE[y][x + 1] && con.IMAGE[y][x - 1])) {
                                CONV.EDGES[y][x] = (uint16)index + 1;
                                SubCON[index].nEdge += 1;
                            }
                        }
                    }
                }
            }

            // ��ͨ���������
            for (index = 0; index < con.count; index++) {
                // >>����Ҫ���ڻ���ڿ�
                // if ((SubCON[index].neX - SubCON[index].nsX + 1) < (SubCON[index].neY - SubCON[index].nsY + 1))
                //    continue;
                // >>��ͨ�������Ҫռ���������һ������
                // if ((SubCON[index].nArea * 2) < ((SubCON[index].neX - SubCON[index].nsX + 1) * (SubCON[index].neY - SubCON[index].nsY + 1)))
                //    continue;
                // >>����Բ�ζ���Ҫ��1����
                // SubCON[index].nRound = (float)(4.0f * 3.1415927f * SubCON[index].nArea) / (float)(SubCON[index].nEdge * SubCON[index].nEdge);
                // if (fabs(SubCON[index].nRound - 1.0f) > 1.0f)
                //    continue;

                cCount = (uint8)index + 1;
            }
        }

        if (cCount) {
            FitCON = &SubCON[cCount - 1];

            look->EUREKA = 1;
            look->count = cCount;

            look->sX = FitCON->nsX;
            look->sY = FitCON->nsY;
            look->eX = FitCON->neX;
            look->eY = FitCON->neY;
            look->cArea = FitCON->nArea;
            look->cEdge = FitCON->nEdge;
            look->cRound = FitCON->nRound;

            look->X = (look->eX + look->sX) / 2;
            look->Y = (look->eY + look->sY) / 2;
        } else {
            look->EUREKA = 0;
            look->X = 0;
            look->Y = 0;
            look->sX = 0;
            look->sY = 0;
            look->eX = 0;
            look->eY = 0;
            look->cArea = 0;
            look->cEdge = 0;
            look->cRound = 0.0f;
            look->count = 0;
        }

        free(SubCON);
    }
}

/**
 * @brief  : Gentle change of coordinates
 * @param  : look:
 */
void Looking_Coordinates(Looking_t *look) {
    static int16 LastX = 0,
                 LastY = 0;

    if (look->X > LastX) {
        LastX += 1;

        if (LastX > IMAGE_W - 1)
            LastX = IMAGE_W - 1;

        look->SoftX = (uint8)(LastX);
    }
    if (look->X < LastX) {
        LastX -= 1;

        if (LastX < 0)
            LastX = 0;

        look->SoftX = (uint8)(LastX);
    }

    if (look->Y > LastY) {
        LastY += 2;

        if (LastY > IMAGE_H - 1)
            LastY = IMAGE_H - 1;

        look->SoftY = (uint8)(LastY);
    }
    if (look->Y < LastY) {
        LastY -= 2;

        if (LastY < 0)
            LastY = 0;

        look->SoftY = (uint8)(LastY);
    }
}

int16 MAP_distortX[IMAGE_H][IMAGE_W];  // ȥ����ͼ��������Ӧ��ԭͼ�������
int16 MAP_distortY[IMAGE_H][IMAGE_W];  // ȥ����ͼ���������Ӧ��ԭͼ��������
/**
 * @brief  : Image distort coordinate map
 */
void Image_Undistort_MAP(void) {
    uint8 v, u;
    double x1, y1, x2, y2, r2;

    for (v = 0; v < IMAGE_H; v++)
        for (u = 0; u < IMAGE_W; u++) {
            x1 = (u - DIS_Cx) / DIS_Fx;
            y1 = (v - DIS_Cy) / DIS_Fy;

            r2 = pow(x1, 2) + pow(y1, 2);
            x2 = x1 * (1 + DIS_K1 * r2 + DIS_K2 * pow(r2, 2)) + 2 * DIS_P1 * x1 * y1;         // + DIS_P2 * (r2 + 2 * x1 * x1);
            y2 = y1 * (1 + DIS_K1 * r2 + DIS_K2 * pow(r2, 2)) + DIS_P1 * (r2 + 2 * y1 * y1);  // + 2 * DIS_P2 * x1 * y1;

            // ����ڲ�ֵ ��������
            MAP_distortX[v][u] = (int16)((DIS_Fx * x2 + DIS_Cx + 0.5f));
            MAP_distortY[v][u] = (int16)((DIS_Fy * y2 + DIS_Cy + 0.5f));
        }
}

/**
 * @brief  : Image undistort (170�����������ͷͼ��ȥ����)
 * @param  : cam:
 */
void Image_Undistort(MT9V034_t *cam) {
    uint8 v, u;
    int16 x, y;

    for (v = 0; v < IMAGE_H; v++)
        for (u = 0; u < IMAGE_W; u++) {
            x = MAP_distortX[v][u];
            y = MAP_distortY[v][u];

            if (x >= 0 && y >= 0 && x < IMAGE_W && y < IMAGE_H)
                cam->UNDST[v][u] = cam->IMAGE[y][x];
            else
                cam->UNDST[v][u] = 0;
        }
}

/**
 * @brief  : Camera image binarization
 * @param  : cam:
 */
void Image_BIN(MT9V034_t *cam) {
    uint8 width, height;
    uint8 *src_img;

    if (Mode_Undistort)
        src_img = *cam->UNDST;
    else
        src_img = *cam->IMAGE;

    for (height = IMAGE_ignoreTop; height < IMAGE_H - IMAGE_ignoreBottom; height++) {
        for (width = 0; width < IMAGE_W; width++) {
            if (*(src_img++) > cam->threshold)
                cam->BIN[height][width] = 1;
            else
                cam->BIN[height][width] = 0;
        }
    }
}

/**
 * @brief  : Camera image mean thresholding
 * @param  : cam:
 */
void Image_Threshold(MT9V034_t *cam) {
    uint8 width, height;
    uint32 PixelSum = 0;
    uint8 *src_img;

    if (Mode_Undistort)
        src_img = *cam->UNDST;
    else
        src_img = *cam->IMAGE;

    for (height = 0; height < IMAGE_H; height++)
        for (width = 0; width < IMAGE_W; width++)
            PixelSum += (uint32) * (src_img++);

    cam->threshold = (uint8)(PixelSum / IMAGE_W / IMAGE_H) + cam->threshold_offset;
}

/**
 * @brief  : Limit OTSU and 2-Mode Method
 * @param  : cam:
 */
void Image_thresholdGray2Binary(MT9V034_t *cam) {
    uint8 *src_img;
    uint8 *img_binary = *cam->BIN;
    float AllGrayScale[256];
    uint16 AllGrayVal[256] = {0};  // ���лҶ�ֵ�ĸ�������
    uint16 i, gray_val;
    float w0, w1;       // ǰ���ͱ��������ص�ռ��
    float u0, u1;       // ǰ�� �����������ƽ���Ҷ�
    float g, maxg = 0;  // ��䷽��

    uint16 MinTop[2] = {0};
    uint16 MaxTop[2] = {0};

    if (Mode_Undistort)
        src_img = *cam->UNDST;
    else
        src_img = *cam->IMAGE;

    for (i = 0; i < IMAGE_SIZE; i++)
        AllGrayVal[*(src_img++)]++;

    for (i = 0; i < 256; i++)
        AllGrayScale[i] = (float)AllGrayVal[i] / IMAGE_SIZE;

    for (gray_val = 0; gray_val < 256; gray_val++)  // �������䷽��ʱ����ֵ
    {
        w0 = w1 = u0 = u1 = 0;
        for (i = 0; i < 256; i++) {
            if (i <= gray_val)  // С����ֵ,ǰ������
            {
                w0 += AllGrayScale[i];
                u0 += i * AllGrayScale[i];
            } else  // ����
            {
                // w1 += AllGrayScale[i];
                u1 += i * AllGrayScale[i];
            }
        }
        w1 = 1.0f - w0;
        u0 /= w0;                             // ǰ�����Ҷ�
        u1 /= w1;                             // �������Ҷ�
        g = w0 * w1 * (u0 - u1) * (u0 - u1);  // g=��0��1(��0-��1)^2
        if (g > maxg && gray_val < 128)       // �и������䷽��
        {
            maxg = g;
            cam->threshold = (uint8)gray_val;  // �ҵ������䷽���Ӧ����ֵ
        }
    }

    // ˫��
    for (i = cam->threshold; i > 0; i--) {
        if (MinTop[1] < AllGrayVal[i]) {
            MinTop[1] = AllGrayVal[i];
            MinTop[0] = i;
            break;
        }
    }
    for (i = cam->threshold; i < 128; i++) {
        if (MaxTop[1] < AllGrayVal[i]) {
            MaxTop[1] = AllGrayVal[i];
            MaxTop[0] = i;
            break;
        }
    }
    cam->threshold = (MinTop[0] + MaxTop[0]) / 2;

    if (cam->threshold < 10)
        cam->threshold = 255;

    if (Mode_Undistort)  // ��ʼʹ����ֵ��ֵ��
        src_img = *cam->UNDST;
    else
        src_img = *cam->IMAGE;

    for (i = 0; i < IMAGE_SIZE; i++) {
        if (*(src_img++) > cam->threshold)
            *(img_binary++) = 1;
        else
            *(img_binary++) = 0;
    }
}
