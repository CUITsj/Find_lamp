#include "init.h"

uint8  nrf_rx_buff[CAMERA_SIZE + 2*COM_LEN + DATA_PACKET];         //Ԥ��
uint8  nrf_tx_buff[CAMERA_SIZE + 2*COM_LEN + DATA_PACKET];         //Ԥ��

uint32 HMI_flash_ex = 0;

void System_Init()
{
  char uart_rx_buf = 0;
  uint8 HMI_rx = 0;
  uint8 NRF_switch = 0;
  
  
  /************************  ��ʼ����ʼ  ************************************/
  led_init(LED_MAX);
  led(LED_MAX,LED_ON);
  
  /************************ �����ʼ�� ************************************/
  ftm_pwm_init(MOTOR_FTM, MOTOR1_CH, MOTOR_HZ, 0);//����ǰ
  ftm_pwm_init(MOTOR_FTM, MOTOR2_CH, MOTOR_HZ, 0);//���ֺ�  
  ftm_pwm_init(MOTOR_FTM, MOTOR3_CH, MOTOR_HZ, 0);//����ǰ
  ftm_pwm_init(MOTOR_FTM, MOTOR4_CH, MOTOR_HZ, 0);//���ֺ�
  
  /************************ ���� K60 �����ȼ�  *****************************/
  NVIC_SetPriorityGrouping(4);      //������Χ��0~4��4��ʾ4bitȫΪ��ռ���ȼ���û��ѹ���ȼ�
  NVIC_SetPriority(PORTA_IRQn,0);         // ����ԽС���ȼ�Խ�ߣ�����ͷ
  NVIC_SetPriority(PORTE_IRQn,1);         // NRF
  NVIC_SetPriority(DMA0_IRQn,2);          // ����ͷ
  NVIC_SetPriority(PIT0_IRQn,3);          // ��ʱ��
  NVIC_SetPriority(PIT1_IRQn,4);          // ��ʱ��
  NVIC_SetPriority(UART4_RX_TX_IRQn,5);   // ����
  
  /************************ ����ͷ ��ʼ�� **********************************/
  camera_init(imgbuff);                                   // ����ͷ��ʼ������ͼ��ɼ��� ImageBuff ��ַ
  set_vector_handler(PORTA_VECTORn ,PORTA_IRQHandler);	// ���� PORTA ���жϷ�����Ϊ PORTA_IRQHandler
  set_vector_handler(DMA0_VECTORn ,DMA0_IRQHandler);      // ���� DMA0 ���жϷ�����Ϊ DMA0_IRQHandler
  
  /************************ ��������ʼ�� **********************************/ 
  ftm_quad_init(FTM1);                                    //FTM1 ���������ʼ�������õĹܽſɲ� port_cfg.h ��
  ftm_quad_init(FTM2);                                    //FTM2 ���������ʼ�������õĹܽſɲ� port_cfg.h ��
                                                               //ʹ�ö�ʱ��0������10msִ��һ��
  
  /************************** UART���� ��ʼ��  ***********************/
  uart_init(UART4, 9600); //���ڴ�����
  
  /************************ ����ģ�� NRF ��ʼ��  ***********************/
  #if 1
    uint32 i = 20;
    while(!nrf_init());
    //�����жϷ�����
    set_vector_handler(PORTE_VECTORn, PORTE_IRQHandler);
    enable_irq(PORTE_IRQn);
    nrf_msg_init();
    
    while(i--)
    {
      nrf_msg_tx(COM_RETRAN, nrf_tx_buff);// COM_RETRAN��λ���䣬����֮ǰ���յ�������
    }
  #endif
  
  /************************ FLASH�ʹ�������ʼ�� *****************************/
  HMI_flash_ex = flash_read(SECTOR_NUM, 0, uint32);
  //��ʾ��ǰ�������ֵ
  printf("t0.txt=\"%d\"",(uint8)(HMI_flash_ex>>24));
  uart_putchar(UART4,0xff);
  uart_putchar(UART4,0xff);
  uart_putchar(UART4,0xff);  
  //��ʾ��ǰ�����תȦ�ٶ�
  printf("t1.txt=\"%d\"",(uint8)(HMI_flash_ex>>16));
  uart_putchar(UART4,0xff);
  uart_putchar(UART4,0xff);
  uart_putchar(UART4,0xff);  
  //��ʾ��ǰ�����׷���ٶ�
  printf("t2.txt=\"%d\"",(uint8)(HMI_flash_ex>>8));
  uart_putchar(UART4,0xff);
  uart_putchar(UART4,0xff);
  uart_putchar(UART4,0xff);  
  //��ʾ��ǰ�����NRF����״̬
  printf("b6.txt=\"%d\"",(uint8)(HMI_flash_ex>>0) & 0x01);
  uart_putchar(UART4,0xff);
  uart_putchar(UART4,0xff);
  uart_putchar(UART4,0xff);  
  
  while(HMI_rx != 0x08)
  {
    if(uart_querychar(UART4,&uart_rx_buf)==1)     //��ѯ����1���ַ������浽 ch��
    {
      HMI_rx = uart_rx_buf;
      //������ֵ
      if(HMI_rx == 0x01)
      {
        HMI_flash_ex += (1<<24);
      }
      if(HMI_rx == 0x02)
      {
        HMI_flash_ex -= (1<<24);
      }
      //����תȦ�ٶ�
      if(HMI_rx == 0x03)
      {
        HMI_flash_ex += (1<<16);
      }
      if(HMI_rx == 0x04)
      {
        HMI_flash_ex -= (1<<16);
      }
      //����׷���ٶ�
      if(HMI_rx == 0x05)
      {
       HMI_flash_ex += (1<<8);
      }
      if(HMI_rx == 0x06)
      {
        HMI_flash_ex -= (1<<8);
      }
      //����NRF
      if(HMI_rx == 0x07)
      {
        NRF_switch = ~NRF_switch;
        if(NRF_switch)
          HMI_flash_ex |= (1<<0);
        else      
          HMI_flash_ex &= ~(1<<0);
      }
      //��ʾ��ǰ��ֵ
      printf("t0.txt=\"%d\"",(uint8)(HMI_flash_ex>>24));
      uart_putchar(UART4,0xff);
      uart_putchar(UART4,0xff);
      uart_putchar(UART4,0xff);
      //��ʾ��ǰתȦ�ٶ�
      printf("t1.txt=\"%d\"",(uint8)(HMI_flash_ex>>16));
      uart_putchar(UART4,0xff);
      uart_putchar(UART4,0xff);
      uart_putchar(UART4,0xff);
      //��ʾ��ǰ׷���ٶ�
      printf("t2.txt=\"%d\"",(uint8)(HMI_flash_ex>>8));
      uart_putchar(UART4,0xff);
      uart_putchar(UART4,0xff);
      uart_putchar(UART4,0xff);    
      //��ʾNRF����״̬
      printf("b6.txt=\"%d\"",(uint8)(HMI_flash_ex>>0) & 0x01);
      uart_putchar(UART4,0xff);
      uart_putchar(UART4,0xff);
      uart_putchar(UART4,0xff);   
    }
  }
  while(!flash_erase_sector(SECTOR_NUM));                     //��������
  while(!flash_write(SECTOR_NUM, 0, HMI_flash_ex));  //д�����ݵ�������ƫ�Ƶ�ַΪ0������һ��д��4�ֽ� 
  
  /************************ ����ͷ��ֵ��ʼ�� *********************************/
  SCCB_WriteByte(OV7725_CNST, (uint8)(HMI_flash_ex>>24));	//�ı�ͼ����ֵ   
  
  /************************ ͼ��ɼ� ***************************************/
  camera_get_img();	// �Ȳɼ�һ��ͼ��
  
  /************************ ��ʱ�� ��ʼ��  *********************************/ 
  pit_init_ms(PIT0, 10);                              	// ��ʼ��PIT0����ʱʱ��Ϊ�� 10ms
  set_vector_handler(PIT0_VECTORn ,PIT0_IRQHandler);	// ����PIT0���жϷ�����Ϊ PIT0_IRQHandler
  enable_irq (PIT0_IRQn); 				// ʹ���ж�
  
  //����ʹPIT1��PIT2�Ķ�ʱʱ��󣬲���ʹ��ʱ����׼ȷ
  pit_init_ms(PIT1, 10);                           	// ��ʼ��PIT1����ʱʱ��Ϊ�� 20ms
  set_vector_handler(PIT1_VECTORn ,PIT1_IRQHandler); 	// ����PIT1���жϷ�����Ϊ PIT1_IRQHandler
  enable_irq (PIT1_IRQn); 				// ʹ���ж�
  
  /************************  ��ʼ������  ************************************/
  led(LED_MAX,LED_OFF);// ��LEDָʾ��		ȫ����ʼ���ɹ�
}