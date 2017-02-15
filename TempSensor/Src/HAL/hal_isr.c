#include "hal_isr.h"


/****************************************************************************
* 名    称：__interrupt void usart1_rx (void)
* 功    能：RF中断处理函数
* 入口参数：无
* 出口参数：无
* 备    注：无
****************************************************************************/
#pragma vector=USART1RX_VECTOR
__interrupt void usart1_rx (void)
{
    static u8   RfTag = 0;
    static char Addr_SN[6];
    static char Addr_SN_Temp[6] = {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};
   
    switch(RfTag)
    {
      case 0:                     //前导符_1
         if( 0xFE == RXBUF1 )
            RfTag = 1;
         else
            RfTag = 0;
         break;
      case 1:                     //前导符_2
         if( 0xFE == RXBUF1 )
           RfTag = 2;
         else
           RfTag = 0;
         break;
      case 2:                     //前导符_3
         if( 0xFE == RXBUF1 )
           RfTag = 3;
         else
           RfTag = 0;
         break;
      case 3:                     //标准帧接收
         RfBuf[RfIndex++] = RXBUF1;
         if( 1 == RfIndex )       //起始符_1判别
         {
            if( 0x68 == RfBuf[0] )
               return;
            else
            {
               RfIndex = 0;
               RfTag = 0;
               return;
            }
         }
         else if( 7 == RfIndex )       //从站地址判别
         {
            memcpy(Addr_SN, (RfBuf + 1), 6);
            /* 判别广播地址还是寻找的本设备的地址 */
            if( (strncmp(Addr_SN, Addr_SN_Temp, 6) == 0) || (strncmp(Addr_SN, FA_SN_ADDR_START, 6) == 0) )
                return;
            else
            {
               RfIndex = 0;
               RfTag = 0;
               return;
            }
         }
         else if( 8 == RfIndex )       //起始符_2判别
         {
            if( 0x68 == RfBuf[7] )
               return;
            else
            {
               RfIndex = 0;
               RfTag = 0;
               return;
            }
         }
         else if( (12 + RfBuf[9]) == RfIndex )    //结束符判别
         {
             if( 0x16 == RfBuf[12 + RfBuf[9] - 1] )
             {
                RfIndex= 0;
                RfTag = 0;
                switch(RfBuf[8]&CMD_WORD)
                {
                    case READ_VERSION:                             //读版本号
                        OSMboxPost(CMDMbox, &CmdOp[0]);
                        break;
                    case PARAMTER_DOWNLOAD:                        //基本参数下载
                        OSMboxPost(CMDMbox, &CmdOp[1]);
                        break;
                    case READ_PARAMTER:                            //读参数
                        OSMboxPost(CMDMbox, &CmdOp[2]);
                        break;
                    #if D_SAMPLE_TDATA    
                    case SAMPLE_TDATA:                             //从站采集数据发送
                        OSMboxPost(CMDMbox, &CmdOp[3]);
                        break;
                    #endif         
                    #if D_RTU_RESPONSE
                    case RTU_RESPONSE:                             //RTU的回应帧
                        OSMboxPost(ResponseMbox, &CmdOp[4]);
                        break; 
                    #endif    
                    case READ_SN_NUM:                              //读SN号
                        OSMboxPost(CMDMbox, &CmdOp[6]);
                        break;
                    case WRITE_SN_NUM:                             //写设备地址
                        OSMboxPost(CMDMbox, &CmdOp[7]);
                        break;
                    case TIME_CHECK:                               //时间校准
                        OSMboxPost(CMDMbox, &CmdOp[8]);
                        break;
                    case READ_CUR_TIME:                            //读取时间
                        OSMboxPost(CMDMbox, &CmdOp[9]);
                        break;
                    case WARNING_SET:                             //上下限报警设置
                        OSMboxPost(CMDMbox, &CmdOp[10]);
                        break;
                    case SSI_SET:                                 //采集、发送间隔设置
                        OSMboxPost(CMDMbox, &CmdOp[11]);
                        break;
                    default:
                        break;
                }              
                return;
             }
             else
             {
                RfIndex = 0;
                RfTag = 0;
                return;
             }
         }
         else
            break;
        default:
            RfTag = 0;
            RfIndex = 0;
            break;
    }
}

/****************************************************************************
* 名    称：__interrupt void usart1_tx (void)
* 功    能：RF中断处理函数
* 入口参数：无
* 出口参数：无
* 备    注：无
****************************************************************************/
extern char *pDataByte;
extern u32  RfDataLength;
#pragma vector=USART1TX_VECTOR
__interrupt void usart1_tx(void)
{
  static u32 SendIndex;
  TXBUF1 = *pDataByte++;
  SendIndex++;
  if( RfDataLength == SendIndex )
  {
    SendIndex = 0;
    RF_UTXDIS;                  //禁能串口发送(RF)
  }
  
}
