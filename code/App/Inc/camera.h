#ifndef __CAMERA_H__
#define __CAMERA_H__

#include "common.h"
#include "include.h"

extern uint8 road_mid;     //ͼ�������յõ��������е�
extern uint8 img[]; 
extern uint8 light_ln;


uint8 find_col();                                //�ҵ��Ƶ�����
uint8 find_ln(uint8 col);                                //�ҵ��Ƶ�����
void img_extract(uint8 *dst, uint8 *src, uint32 srclen);        //��ѹ

#endif