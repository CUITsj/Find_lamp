#include "camera.h"

uint8 colour[2] = {1, 0}; //0��ʾ�ڡ�1��ʾ�죬�˴����Ըı��ʾ�ڰ׵�����

uint8 find_col()//0��ʾ��1��ʾ��
{
  
  /********************* ͼ��ɨ������ʾ��ͼ **********************
   ___________________________________________________________
  |     scan_upleft->|____scan_downbound____|<-scan_upright    |
  |  ________________|___scan_dividbound__|________________  |
  |  |                                                    |  |
  |  |                                                    |  |
  |  |                                                    |  |
  |  |<-scan_downleft                     scan_downright->|  |
  |  |                                                    |  |
  |  |                                                    |  |
  |  |                                                    |  |
  |  |____________________scan_upbound__________________|  |
  |__________________________________________________________|

   ************************* ending ****************************/
  
  uint8 scan_point, scan_upbound = (uint8)(HMI_flash_ex>>16), scan_downbound = 3, scan_dividbound = 15; //ɨ�����½�
  uint8 scan_upleft = 20, scan_upright = 60, scan_downleft = 10, scan_downright = 70;
  uint8 get_right_flag[61], get_left_flag[61], get_mid_flag[61]; //���߱�־
  int8 i, j;
  int8 left_bound[61], right_bound[61], mid_point[61];
  uint16 midpoint_sum = 0;
  uint8 midpoint_num = 0;
  uint8 road_midpoint;
  
  for(i=scan_downbound; i<=scan_upbound; i++)//��9��scan_upbound����ɨ��
  {
    get_right_flag[i] = 0;
    get_left_flag[i] = 0;
    get_mid_flag[i] = 0; 
    
   if (i < scan_dividbound)
   {
     /********************************** ��ʼ�ҵƵ���߽� **************************************/
    for(j=scan_upleft,scan_point=scan_upleft; j<=scan_upright; j++)//��ʼ����߽�
    {
      if (img[i*80 + j] == 0 && img[i*80 + j + 1] == 1)//�ҵ��˱߽�
      {
        left_bound[i] = j - 1;//�ҵ�����߽�
        get_left_flag[i] = 1;          
        break;
      }
      else if (img[i*80 + scan_point] == 0 && img[i*80 + scan_point + 1] == 0)//ȫ��
      {
        if (scan_point == scan_upright)  //����û����߽�
        {
          get_left_flag[i] = 0;
          left_bound[i] = 0;    //ȫ�ڶ��ߣ�Ĭ��0
          break;
        }
        scan_point++;
      }
      else if(img[i*80 + j] == 1)//�������ʼ����
      {
          get_left_flag[i] = 1;
          left_bound[i] = scan_upleft;  
          break;
      }
    }
/********************************** �ҵ���߽���� **************************************/
/********************************** ��ʼ�ҵƵ��ұ߽� **************************************/
    for(j=scan_upright,scan_point=scan_upright; j>=scan_upleft; j--)//��ʼ���ұ߽�
    {
      if (img[i*80 + j] == 0 && img[i*80 + j - 1] == 1)
      {
        get_right_flag[i] = 1;
        right_bound[i] = j + 1;
        break; 
      }
      else if (img[i*80 + scan_point] == 0 && img[i*80 + scan_point - 1] == 0)//ȫ��
      {
        if (scan_point == scan_upleft) // �������ұ߽�
        {
          right_bound[i] = 0;//ȫ�ڶ��ߣ�Ĭ��0
          get_right_flag[i] = 0;
          break;
        }
        scan_point--;
      }
      else if (img[i*80 + j] == 1)//�ұ߽�����ʼ����
      {
        get_right_flag[i] = 1;
        right_bound[i] = scan_upright;
        break; 
      }
    }            
/********************************** �ҵƵ��ұ߽���� **************************************/          
   }
   else if (i >= scan_dividbound)
   {
      /********************************** ��ʼ�ҵƵ���߽� **************************************/
          for(j=scan_downleft,scan_point=scan_downleft; j<=scan_downright; j++)//��ʼ����߽�
          {
            if (img[i*80 + j] == 0 && img[i*80 + j + 1] == 1)//�ҵ��˱߽�
            {
              left_bound[i] = j - 1;//�ҵ�����߽�
              get_left_flag[i] = 1;          
              break;
            }
            else if (img[i*80 + scan_point] == 0 && img[i*80 + scan_point + 1] == 0)//ȫ��
            {
              if (scan_point == scan_downright)  //����û����߽�
              {
                get_left_flag[i] = 0;
                left_bound[i] = 0;    //ȫ�ڶ��ߣ�Ĭ��0
                break;
              }
              scan_point++;
            }
            else if(img[i*80 + j] == 1)//�������ʼ����
            {
                get_left_flag[i] = 1;
                left_bound[i] = scan_downleft;  
                break;
            }
          }
      /********************************** �ҵ���߽���� **************************************/
      /********************************** ��ʼ�ҵƵ��ұ߽� **************************************/
          for(j=scan_downright,scan_point=scan_downright; j>=scan_downleft; j--)//��ʼ���ұ߽�
          {
            if (img[i*80 + j] == 0 && img[i*80 + j - 1] == 1)
            {
              get_right_flag[i] = 1;
              right_bound[i] = j + 1;
              break; 
            }
            else if (img[i*80 + scan_point] == 0 && img[i*80 + scan_point - 1] == 0)//ȫ��
            {
              if (scan_point == scan_downleft) // �������ұ߽�
              {
                right_bound[i] = 0;//ȫ�ڶ��ߣ�Ĭ��0
                get_right_flag[i] = 0;
                break;
              }
              scan_point--;
            }
            else if (img[i*80 + j] == 1)//�ұ߽�����ʼ����
            {
              get_right_flag[i] = 1;
              right_bound[i] = scan_downright;
              break; 
            }
          }            
      /********************************** �ҵƵ��ұ߽���� **************************************/              
   }
   /********************************** ����������߽�� **************************************/
          if (get_left_flag[i] == 1 && get_right_flag[i] == 1)              //�õ����ұ߽��е�
          {
            mid_point[i] = (left_bound[i] + right_bound[i])/2;
            get_mid_flag[i] = 1;
          }
      /********************************** �������߽��������� **************************************/
        }//������һ�� 
 for (i=scan_downbound; i<=scan_upbound; i++)
 {
     if (get_mid_flag[i] == 1)
      {
        midpoint_sum += mid_point[i];
        midpoint_num++;
      }
 }
 
 if (midpoint_num == 0)
 {
   road_midpoint = 0;
 }
 else if (midpoint_num > 0)
 {
   road_midpoint = midpoint_sum/midpoint_num;
 }
 return road_midpoint;
}

uint8 find_ln(uint8 col)//0��ʾ��1��ʾ��
{
  uint8 i,upbound = 0, downbound = 0;
  
  for(i=3; i<=(uint8)(HMI_flash_ex>>16); i++)//��ʼ����߽�
  {
    if (img[i*80 + col] == 0 && img[(i+1)*80 + col] == 1)//�ҵ��˱߽�
    {
      upbound = i;
    }
    if (img[i*80 + col] == 1 && img[(i+1)*80 + col] == 0)//�ҵ��˱߽�
    {
      downbound = i;
      break;
    }
  }
  if (upbound == 0 || downbound == 0)
  {
    for(i=3; i<=(uint8)(HMI_flash_ex>>16); i++)//��ʼ����߽�
    {
      if (img[i*80 + col - 1] == 0 && img[(i+1)*80 + col - 1] == 1)//�ҵ��˱߽�
      {
        upbound = i;
      }
      if (img[i*80 + col - 1] == 1 && img[(i+1)*80 + col - 1] == 0)//�ҵ��˱߽�
      {
        downbound = i;
        break;
      }
    }
  }
  if (upbound == 0 || downbound == 0)
  {
    for(i=3; i<=(uint8)(HMI_flash_ex>>16); i++)//��ʼ����߽�
    {
      if (img[i*80 + col + 1] == 0 && img[(i+1)*80 + col + 1] == 1)//�ҵ��˱߽�
      {
        upbound = i;
      }
      if (img[i*80 + col + 1] == 1 && img[(i+1)*80 + col + 1] == 0)//�ҵ��˱߽�
      {
        downbound = i;
        break;
      }
    }
  }
  
    
  return (upbound + downbound)/2;
}

/*!
*  @brief      ��ֵ��ͼ���ѹ���ռ� �� ʱ�� ��ѹ��
*  @param      dst             ͼ���ѹĿ�ĵ�ַ
*  @param      src             ͼ���ѹԴ��ַ
*  @param      srclen          ��ֵ��ͼ���ռ�ÿռ��С
*  @since      v5.0            img_extract(img, imgbuff,CAMERA_SIZE);
*  Sample usage:
*/
void img_extract(uint8 *dst, uint8 *src, uint32 srclen)
{
  uint8 tmpsrc;
  while(srclen --)
  {
    tmpsrc = *src++;
    *dst++ = colour[ (tmpsrc >> 7 ) & 0x01 ];
    *dst++ = colour[ (tmpsrc >> 6 ) & 0x01 ];
    *dst++ = colour[ (tmpsrc >> 5 ) & 0x01 ];
    *dst++ = colour[ (tmpsrc >> 4 ) & 0x01 ];
    *dst++ = colour[ (tmpsrc >> 3 ) & 0x01 ];
    *dst++ = colour[ (tmpsrc >> 2 ) & 0x01 ];
    *dst++ = colour[ (tmpsrc >> 1 ) & 0x01 ];
    *dst++ = colour[ (tmpsrc >> 0 ) & 0x01 ];
  }
}

