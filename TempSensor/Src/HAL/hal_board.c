#include "hal_board.h"


/****************************************************************************
* ��    �ƣ�void msp430_MCLK_Init(void)
* ��    �ܣ�ʹ���ⲿ������Ϊ��ϵͳʱ��(MCLK)
* ��ڲ�������
* ���ڲ�������
* ��    ע������ʹ��8M��Ϊ��ϵͳʱ��
****************************************************************************/
void msp430_MCLK_Init(void)
{
  unsigned int i;
  BCSCTL1 &= ~XT2OFF;                       // XT2= HF XTAL

  do
  {
  IFG1 &= ~OFIFG;                           // Clear OSCFault flag
  for (i = 0xFF; i > 0; i--);               // Time for flag to set
  }
  while ((IFG1 & OFIFG));                   // OSCFault flag still set?

  BCSCTL2 |= SELM_2 + SELS;                 // MCLK= XT2 (safe)
}

/****************************************************************************
* ��    �ƣ�void UartRf_Init(unsigned long baud)
* ��    �ܣ���Ƶ���ڳ�ʼ��
* ��ڲ�����������
* ���ڲ�������
* ��    ע����
****************************************************************************/
void UartRf_Init(unsigned long baud)
{
  (void) baud;
  P3SEL |= BIT6 + BIT7;                     // P3.6,7 = USART1 TXD/RXD
  P3DIR |= BIT6;
  UCTL1 |= SWRST + CHAR;                    // 8-bit character
  ME2   |= UTXE1 + URXE1;                     // Enable USART1 TXD/RXD

  UTCTL1 = SSEL0;                           //ʹ��ACLK�ⲿ����32768��Ϊ����ʱ��
  UBR01  = 0x03;                            //9600
  UBR11  = 0x00;
  UMCTL1 = 0x4A;
  
  UCTL1 &= ~SWRST;                          // Initialize USART state machine
  _EINT();
  IE2 |= URXIE1 ;                           // Enable USART1 RX interrupt
}


/****************************************************************************
* ��    �ƣ�void Uart_Rf_SendByte(char data)
* ��    �ܣ����ڷ�����ѯ��ʽ
* ��ڲ������ֽ�����
* ���ڲ�������
* ��    ע��Ч�ʵ�
****************************************************************************/
void Uart_Rf_SendByte(char data)
{
    U1TXBUF = data;
}


/****************************************************************************
* ��    �ƣ�void Uart_Rf_SendByte(char data)
* ��    �ܣ����ڷ�����ѯ��ʽ
* ��ڲ������ֽ�����
* ���ڲ�������
* ��    ע��Ч�ʵ�
****************************************************************************/
/*void Uart_Rf_SendByte(char data)
{
    while(0 == (IFG2 & UTXIFG1));
    U1TXBUF = data;
}*/


 
/****************************************************************************
* ��    �ƣ�void  OS_TickStart (void)
* ��    �ܣ�ϵͳʱ������
* ��ڲ�������
* ���ڲ�������
* ��    ע��ʱ�ӽ���ʹ�ÿ��Ź��жϼ��Ϊ125ms
****************************************************************************/
void  OS_TickStart (void)
{
    CCTL0 = CCIE;                         // CCR0 interrupt enabled
    CCR0  = 0x1000;                       // 125ms
    TACTL = TASSEL_1 + MC_1;              // ACLK, upmode                
}

/****************************************************************************
* ��    �ƣ�void LedInit(void)
* ��    �ܣ�ϵͳ����ָʾ��
* ��ڲ�������
* ���ڲ�������
* ��    ע������ʱʹ��
****************************************************************************/
#if LED_DEBUG
void LedInit(void)
{
  P1DIR |= BIT4;
  LED_OFF;
}
#endif

/****************************************************************************
* ��    �ƣ�void System_Init(void)
* ��    �ܣ�ϵͳ��ʼ��
* ��ڲ�������
* ���ڲ�������
* ��    ע����
****************************************************************************/
void BSP_Init(void)
{    
  msp430_MCLK_Init();           //��ʼ��ϵͳʱ��
#if LED_DEBUG
   LedInit();                   //ϵͳ����ָʾ��
#endif
   UartRf_Init(9600);           //RF���ڳ�ʼ��
}