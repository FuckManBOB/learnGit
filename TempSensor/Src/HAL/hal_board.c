#include "hal_board.h"


/****************************************************************************
* 名    称：void msp430_MCLK_Init(void)
* 功    能：使用外部晶振作为主系统时钟(MCLK)
* 入口参数：无
* 出口参数：无
* 备    注：这里使用8M作为主系统时钟
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
* 名    称：void UartRf_Init(unsigned long baud)
* 功    能：射频串口初始化
* 入口参数：波特率
* 出口参数：无
* 备    注：无
****************************************************************************/
void UartRf_Init(unsigned long baud)
{
  (void) baud;
  P3SEL |= BIT6 + BIT7;                     // P3.6,7 = USART1 TXD/RXD
  P3DIR |= BIT6;
  UCTL1 |= SWRST + CHAR;                    // 8-bit character
  ME2   |= UTXE1 + URXE1;                     // Enable USART1 TXD/RXD

  UTCTL1 = SSEL0;                           //使用ACLK外部晶振32768作为串口时钟
  UBR01  = 0x03;                            //9600
  UBR11  = 0x00;
  UMCTL1 = 0x4A;
  
  UCTL1 &= ~SWRST;                          // Initialize USART state machine
  _EINT();
  IE2 |= URXIE1 ;                           // Enable USART1 RX interrupt
}


/****************************************************************************
* 名    称：void Uart_Rf_SendByte(char data)
* 功    能：串口发送轮询方式
* 入口参数：字节数据
* 出口参数：无
* 备    注：效率低
****************************************************************************/
void Uart_Rf_SendByte(char data)
{
    U1TXBUF = data;
}


/****************************************************************************
* 名    称：void Uart_Rf_SendByte(char data)
* 功    能：串口发送轮询方式
* 入口参数：字节数据
* 出口参数：无
* 备    注：效率低
****************************************************************************/
/*void Uart_Rf_SendByte(char data)
{
    while(0 == (IFG2 & UTXIFG1));
    U1TXBUF = data;
}*/


 
/****************************************************************************
* 名    称：void  OS_TickStart (void)
* 功    能：系统时钟配置
* 入口参数：无
* 出口参数：无
* 备    注：时钟节拍使用看门狗中断间隔为125ms
****************************************************************************/
void  OS_TickStart (void)
{
    CCTL0 = CCIE;                         // CCR0 interrupt enabled
    CCR0  = 0x1000;                       // 125ms
    TACTL = TASSEL_1 + MC_1;              // ACLK, upmode                
}

/****************************************************************************
* 名    称：void LedInit(void)
* 功    能：系统调试指示灯
* 入口参数：无
* 出口参数：无
* 备    注：调试时使用
****************************************************************************/
#if LED_DEBUG
void LedInit(void)
{
  P1DIR |= BIT4;
  LED_OFF;
}
#endif

/****************************************************************************
* 名    称：void System_Init(void)
* 功    能：系统初始化
* 入口参数：无
* 出口参数：无
* 备    注：无
****************************************************************************/
void BSP_Init(void)
{    
  msp430_MCLK_Init();           //初始化系统时钟
#if LED_DEBUG
   LedInit();                   //系统调试指示灯
#endif
   UartRf_Init(9600);           //RF串口初始化
}