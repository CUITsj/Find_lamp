#include "common.h"
#include "include.h"

uint8  nrf_rx_buff[CAMERA_SIZE + 2*COM_LEN + DATA_PACKET];         //Ԥ��
uint8 *imgbuff = (uint8 *)(((uint8 *)&nrf_rx_buff) + COM_LEN);     //ͼ���ַ

//��������
void PIT0_IRQHandler();
void PORTE_IRQHandler();

uint8  var1 = 0, var2;
uint16 var3, var4;
uint32 var5=0, var6=0;
uint8  mid_point;

Site_t site     = {120, 90};                           //��ʾͼ�����Ͻ�λ��
Size_t size     = {80, 60};                      //��ʾ����ͼ���С
static FATFS fs;    //�ļ�ϵͳ
static FIL   src;   //�ļ�
uint8 zoomflag = 0;

void  main(void)
{
  
  pit_init_ms(PIT1, 100);                           	// ��ʼ��PIT1����ʱʱ��Ϊ�� 20ms
  set_vector_handler(PIT1_VECTORn ,PIT1_IRQHandler); 	// ����PIT1���жϷ�����Ϊ PIT1_IRQHandler
  enable_irq (PIT1_IRQn); 				// ʹ���ж�
   
    Size_t imgsize  = {CAMERA_W, CAMERA_H};             //ͼ���С

    com_e     com;
    nrf_result_e nrf_result;

    /************************ ���� K60 �����ȼ�  ***********************/
    NVIC_SetPriorityGrouping(4);            //�������ȼ�����,4bit ��ռ���ȼ�,û�������ȼ�
    NVIC_SetPriority(PORTE_IRQn,0);         //�������ȼ�
    NVIC_SetPriority(PIT0_IRQn,1);          //�������ȼ�

    /************************ LCD Һ���� ��ʼ��  ***********************/
    LCD_init();

    /************************ ����ģ�� NRF ��ʼ��  ***********************/
    while(!nrf_init());
    //�����жϷ�����
    set_vector_handler(PORTE_VECTORn ,PORTE_IRQHandler);//���� PORTE ���жϷ�����Ϊ PORTE_VECTORn
    enable_irq(PORTE_IRQn);
    nrf_msg_init();                                     //����ģ����Ϣ��ʼ��

    /*********************** ������Ϣ ��ʼ��  ***********************/
//    key_event_init(KEY_MAX);                 //������Ϣ��ʼ��
//    pit_init_ms(PIT0,10);               //pit ��ʱ�ж�(���ڰ�����ʱɨ��)
//    set_vector_handler(PIT0_VECTORn ,PIT0_IRQHandler);    //���� PIT0 ���жϷ�����Ϊ PIT0_IRQHandler
//    enable_irq(PIT0_IRQn);
    
    site.x = 116;
    site.y = 86;
    size.W = 88;
    size.H = 68;
    LCD_rectangle(site, size, WHITE);
    site.x = 120;
    site.y = 90;
    size.W = 80;
    size.H = 60;

    while(1)
    {
        /************************ ���߷��ͺͽ�������  ***********************/
        do
        {
            nrf_result = nrf_msg_rx(&com, nrf_rx_buff);
            if(nrf_result == NRF_RESULT_RX_VALID)
            {
                switch(com)
                {
                    case COM_IMG:
                      LCD_Img_Binary_Z(site,size, imgbuff,imgsize);
                      break;
                    case COM_VAR:  
                      site.x = 120;
                      site.y = 90+var2;
                      size.W = 80;
                      size.H = 1;
                      LCD_rectangle(site, size, GREEN);//��ʾ��
                      site.x = 120+var1;
                      site.y = 90;
                      size.W = 1;
                      size.H = 60;
                      LCD_rectangle(site, size, GREEN);//��ʾ��
//                      site.x = 120+var3;
//                      site.y = 90;
//                      size.W = 1;
//                      size.H = 60;
//                      LCD_rectangle(site, size, GREEN);//��ʾ��
//                      site.x = 120+var4;
//                      site.y = 90;
//                      size.W = 1;
//                      size.H = 60;
//                      LCD_rectangle(site, size, GREEN);//��ʾ��
//                      site.x = 120;
//                      site.y = 90+var5;
//                      size.W = 80;
//                      size.H = 1;
//                      LCD_rectangle(site, size, GREEN);//��ʾ��
                      site.x = 120;
                      site.y = 90;
                      size.W = 80;
                      size.H = 60;
                      break;
                    default:
                      break;
                }
            }
        }while(nrf_result != NRF_RESULT_RX_NO);  
        /*********************** ������Ϣ ����  ***********************/
        //deal_key_event();
    }
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
 *  @brief      PIT0�жϷ�����
 *  @since      v5.0
 */
void PIT0_IRQHandler()
{
    key_IRQHandler();

    PIT_Flag_Clear(PIT0);
}
void deal_key_event()
{
    KEY_MSG_t keymsg;

    while(get_key_msg(& keymsg))     //��ð����ͽ��д���
    {
        if(keymsg.status == KEY_DOWN)
        {
            switch(keymsg.key)
            {
            case KEY_U:
              
                break;

            case KEY_D:
              
                break;

            case KEY_L:
                save_var(CAR_CTRL,CAR_STOP);
                if (var_syn(CAR_CTRL) == 1)
                {
                    site.x = 80;
                    site.y = 60;
                    LCD_str(site, "STOP!", FCOLOUR, BCOLOUR);
                    site.x = 0;
                    site.y = 0;
                }       
                break;

            case KEY_R:
               save_var(CAR_CTRL,CAR_START);
                if (var_syn(CAR_CTRL) == 1)
                {
                    site.x = 80;
                    site.y = 60;
                    LCD_str(site, "START!", FCOLOUR, BCOLOUR);
                    site.x = 0;
                    site.y = 0;
                }
                break;

            case KEY_A:
                break;

            case KEY_START:
                
                break;
            case KEY_STOP:
                      
                break;

            default:
                break;
            }
        }
        else if(keymsg.status == KEY_HOLD)
        {
            switch(keymsg.key)
            {
            case KEY_U:
                
                break;

            case KEY_D:
               
                break;

            case KEY_L:
                save_var(CAR_CTRL,CAR_STOP);
                while(var_syn(CAR_CTRL)==0);
                site.x = 80;
                site.y = 60;
                LCD_str(site, "STOP!", FCOLOUR, BCOLOUR);
                site.x = 0;
                site.y = 0;
                break;

            case KEY_R:
               save_var(CAR_CTRL,CAR_START);
                while(var_syn(CAR_CTRL)==0);   
                site.x = 80;
                site.y = 60;
                LCD_str(site, "START!", FCOLOUR, BCOLOUR);
                site.x = 0;
                site.y = 0;
                break;

            case KEY_A:   

                break;

            case KEY_START:
                
                break;

            case KEY_STOP:
                
                break;

            default:            //����Ҫ���� cancel
                break;
            }
        }
    }
}

void img_sd_init(void)
{
    int     res;
    char    myfilename[20];
    uint16  imgsize[] = {
                            CAMERA_H,
                            CAMERA_W,
                            //CAMERA_COLOR,
                         };      //�ߡ�������ͷ��ɫ����
    uint32  mybw;
    uint32  Imag_num = 0;

    
    f_mount(0, &fs);

    do
    {
        Imag_num ++;
        sprintf(myfilename, "0:/FIRE%d.sd", Imag_num);
        res = f_open( &src , myfilename, FA_CREATE_NEW | FA_WRITE);
        if( res == FR_DISK_ERR)
        {
          site.x = 0;
          site.y = 60;
          LCD_str(site, "no sdcard!", FCOLOUR, BCOLOUR);
          site.x = 0;
          site.y = 0;
        }
        if(fs.fs_type == 0)
        {
            src.fs = 0;
            return;
        }

    }while(res == FR_EXIST);        //����ļ����ڣ�������������1

    if ( res == FR_OK )
    {
      site.x = 0;
      site.y = 60;
      LCD_str(site, "open success!", FCOLOUR, BCOLOUR);
      site.x = 0;
      site.y = 0;
      res = f_write(&src, imgsize, sizeof(imgsize),&mybw);  //��д��ߺͿ�������λ������
    }
    else
    {
        f_close(&src);
        src.fs = 0;
    }
}




void img_sd_save(uint8 * imgaddr,uint32 size)
{
#define F_SYNC_TIME   30      //���� n �κ� �� ͬ��

    int   res;
    uint32 mybw;
    static uint8 time = 0;
    //uint32 size = CAMERA_SIZE;

    if(src.fs != 0)
    {
        time ++;

        res = f_write(&src, imgaddr, size ,&mybw);

        if(res != FR_OK)
        {
          site.x = 0;
          site.y = 100;
          LCD_str(site, "write failed!", FCOLOUR, BCOLOUR);
          site.x = 0;
          site.y = 0;
          f_close(&src);
          src.fs = 0;
        }

        if(time > F_SYNC_TIME)
        {
            time = 0 ;
            f_sync(&src);
        }
    }
}

void img_sd_exit(void)
{
    f_close(&src);
    src.fs = 0;
}
