/*!
*     COPYRIGHT NOTICE
*     Copyright (c) 2013,ɽ��Ƽ�
*     All rights reserved.
*     �������ۣ�ɽ����̳ http://www.vcan123.com
*
*     ��ע�������⣬�����������ݰ�Ȩ����ɽ��Ƽ����У�δ����������������ҵ��;��
*     �޸�����ʱ���뱣��ɽ��Ƽ��İ�Ȩ������
*
* @file       MK60_it.c
* @brief      ɽ��K60 ƽ̨�жϷ�����
* @author     ɽ��Ƽ�
* @version    v5.0
* @date       2013-06-26
*/
#include    "MK60_it.h"

/*********************************�жϷ�����********************************/

/*!
*  @brief      PORTA�жϷ�����
*  @since      v5.0
*/
void PORTA_IRQHandler()
{
  uint8  n;    //���ź�
  uint32 flag;
  
  flag = PORTA_ISFR;
  PORTA_ISFR  = ~0;                                   //���жϱ�־λ
  
  n = 29;                                             //���ж�
  if(flag & (1 << n))                                 //PTA29�����ж�
  {
    camera_vsync();
  }
#if ( CAMERA_USE_HREF == 1)                            //ʹ�����ж�
  n = 28;
  if(flag & (1 << n))                                 //PTA28�����ж�
  {
    camera_href();
  }
#endif
}

/*!
*  @brief      PORTE�жϷ�����
*  @since      v5.0
*/
void PORTE_IRQHandler()
{
  uint8  n;    //���ź�
  uint32 flag;
  
  flag = PORTE_ISFR;
  PORTE_ISFR  = ~0;                                   //���жϱ�־λ
  
  n = 27;
  if(flag & (1 << n))                                 //PTE27�����ж�
  {
    nrf_handler();
  }
}

/*!
*  @brief      DMA0�жϷ�����
*  @since      v5.0
*/
void DMA0_IRQHandler()
{
  camera_dma();
}

/*!
*  @brief      PIT0�жϷ�����
*  @since      v5.0
*/
void PIT0_IRQHandler()	// 10ms��һ���ж�
{
  MOTOR_measure();
  Left_motor_pid(Left_motor_speed);
  Right_motor_pid(Right_motor_speed);

  ftm_pwm_duty(MOTOR_FTM, MOTOR1_CH, MOTOR1_DUTY);//����ǰ
  ftm_pwm_duty(MOTOR_FTM, MOTOR2_CH, MOTOR2_DUTY);//���ֺ�  
  ftm_pwm_duty(MOTOR_FTM, MOTOR3_CH, MOTOR3_DUTY);//����ǰ
  ftm_pwm_duty(MOTOR_FTM, MOTOR4_CH, MOTOR4_DUTY);//���ֺ� 
  
  PIT_Flag_Clear(PIT0);//�����ʱ���жϱ�־λ
}


/*!
*  @brief      PIT1�жϷ�����
*  @since      v5.0
*/
void PIT1_IRQHandler()
{
  
  PIT_Flag_Clear(PIT1);//�����ʱ���жϱ�־λ
}