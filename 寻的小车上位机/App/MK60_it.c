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

/*!
*  @brief      PIT1�жϷ�����
*  @since      v5.0
*/
void PIT1_IRQHandler()
{

    updata_var(VAR1);
    var_display(VAR1); 
    updata_var(VAR2);
    var_display(VAR2); 
//    updata_var(VAR3);
//    var_display(VAR3); 
//    updata_var(VAR4);
//    var_display(VAR4); 
//    updata_var(VAR5);
//    var_display(VAR5); 
  
  PIT_Flag_Clear(PIT1);//�����ʱ���жϱ�־λ
}

