#ifndef __INIT_H__
#define __INIT_H__
#include "common.h"
#include "include.h"

#define SECTOR_NUM  (FLASH_SECTOR_NUM-1)



extern uint8 * imgbuff;             //���ڴ洢NRF�����ͼ��
extern uint8 nrf_rx_buff[];         //Ԥ��,���ڴ洢NRF�����ͼ��
extern uint8 nrf_tx_buff[];         //Ԥ��,���ڴ洢NRF�����ͼ��
extern uint32 HMI_flash_ex;

void System_Init();		 //����ģ���ʼ��

#endif