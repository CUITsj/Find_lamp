#include "common.h"
#include "include.h"

uint8 img[CAMERA_W*CAMERA_H]; //����װ��ѹ���ͼ�񣬼�����ͼ�������ͼ��
uint8 *imgbuff = (uint8 *)(((uint8 *)&nrf_tx_buff) + COM_LEN);	// ���ڴ洢NRF�����ͼ��
uint8 road_mid = 40;     //ͼ������õ��������е�
uint8 light_ln = 0;

uint8  var1, var2;
uint16 var3, var4;
uint32 var5, var6;

uint8 Orbit_speed = 0;
uint8 Run_speed = 0;
uint8 flag = 0;

/*********** main����***********/
void main(void)
{         
  System_Init();	        //��ʼ������ģ��
  Orbit_speed = 20;
  Run_speed = (uint8)(HMI_flash_ex>>8);
  
  while(1)
  {
    camera_get_img();                            // ����ͷ��ȡͼ��
    while (!Image_Flag);                        // �ɼ����
    img_extract(img, imgbuff, CAMERA_SIZE);
    road_mid = find_col();
    if (road_mid != 0)
    {
      light_ln = find_ln(road_mid);//0��ʾ��1��ʾ��
      if (flag == 0)
      {
        flag = 1;
        orbit_speed(0);
        while(MOTOR1_speed != 0 || MOTOR2_speed != 0);//�ȴ�С��ͣ��
        run_speed(Run_speed);
      }
      else
      {
        light_ln = find_ln(road_mid);//0��ʾ��1��ʾ��
        run_speed(Run_speed);
      }
    }
    else
    {
      if (flag == 1)
      {
        light_ln = 0;//û�ҵ�����ҲҪ����
        flag = 0;
        orbit_speed(0);
        while(MOTOR1_speed != 0 || MOTOR2_speed != 0);//�ȴ�С��ͣ��
        Left_motor_speed = -50;
        Right_motor_speed = -50;
        DELAY_MS(700);
        orbit_speed(0);
        while(MOTOR1_speed != 0 || MOTOR2_speed != 0);
        
        orbit_speed(Orbit_speed);
      }
      else
      {
        light_ln = 0;//û�ҵ�����ҲҪ����
        flag = 0;
        orbit_speed(Orbit_speed);
      }
    }
    if (((uint8)(HMI_flash_ex>>0) & 0x01))
    {
      var1 = road_mid;
      var2 = light_ln;
      save_var(VAR1,var1);
      var_syn(VAR1);
      save_var(VAR2,var2);
      var_syn(VAR2);
      nrf_msg_tx(COM_IMG,nrf_tx_buff);
      while(nrf_tx_state() == NRF_TXING);  //�ȴ��������
    }
  }
}
