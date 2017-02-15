#include "hal_isr.h"


/****************************************************************************
* ��    �ƣ�__interrupt void usart1_rx (void)
* ��    �ܣ�RF�жϴ�����
* ��ڲ�������
* ���ڲ�������
* ��    ע����
****************************************************************************/
#pragma vector=USART1RX_VECTOR
__interrupt void usart1_rx (void)
{
    static u8   RfTag = 0;
    static char Addr_SN[6];
    static char Addr_SN_Temp[6] = {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};
   
    switch(RfTag)
    {
      case 0:                     //ǰ����_1
         if( 0xFE == RXBUF1 )
            RfTag = 1;
         else
            RfTag = 0;
         break;
      case 1:                     //ǰ����_2
         if( 0xFE == RXBUF1 )
           RfTag = 2;
         else
           RfTag = 0;
         break;
      case 2:                     //ǰ����_3
         if( 0xFE == RXBUF1 )
           RfTag = 3;
         else
           RfTag = 0;
         break;
      case 3:                     //��׼֡����
         RfBuf[RfIndex++] = RXBUF1;
         if( 1 == RfIndex )       //��ʼ��_1�б�
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
         else if( 7 == RfIndex )       //��վ��ַ�б�
         {
            memcpy(Addr_SN, (RfBuf + 1), 6);
            /* �б�㲥��ַ����Ѱ�ҵı��豸�ĵ�ַ */
            if( (strncmp(Addr_SN, Addr_SN_Temp, 6) == 0) || (strncmp(Addr_SN, FA_SN_ADDR_START, 6) == 0) )
                return;
            else
            {
               RfIndex = 0;
               RfTag = 0;
               return;
            }
         }
         else if( 8 == RfIndex )       //��ʼ��_2�б�
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
         else if( (12 + RfBuf[9]) == RfIndex )    //�������б�
         {
             if( 0x16 == RfBuf[12 + RfBuf[9] - 1] )
             {
                RfIndex= 0;
                RfTag = 0;
                switch(RfBuf[8]&CMD_WORD)
                {
                    case READ_VERSION:                             //���汾��
                        OSMboxPost(CMDMbox, &CmdOp[0]);
                        break;
                    case PARAMTER_DOWNLOAD:                        //������������
                        OSMboxPost(CMDMbox, &CmdOp[1]);
                        break;
                    case READ_PARAMTER:                            //������
                        OSMboxPost(CMDMbox, &CmdOp[2]);
                        break;
                    #if D_SAMPLE_TDATA    
                    case SAMPLE_TDATA:                             //��վ�ɼ����ݷ���
                        OSMboxPost(CMDMbox, &CmdOp[3]);
                        break;
                    #endif         
                    #if D_RTU_RESPONSE
                    case RTU_RESPONSE:                             //RTU�Ļ�Ӧ֡
                        OSMboxPost(ResponseMbox, &CmdOp[4]);
                        break; 
                    #endif    
                    case READ_SN_NUM:                              //��SN��
                        OSMboxPost(CMDMbox, &CmdOp[6]);
                        break;
                    case WRITE_SN_NUM:                             //д�豸��ַ
                        OSMboxPost(CMDMbox, &CmdOp[7]);
                        break;
                    case TIME_CHECK:                               //ʱ��У׼
                        OSMboxPost(CMDMbox, &CmdOp[8]);
                        break;
                    case READ_CUR_TIME:                            //��ȡʱ��
                        OSMboxPost(CMDMbox, &CmdOp[9]);
                        break;
                    case WARNING_SET:                             //�����ޱ�������
                        OSMboxPost(CMDMbox, &CmdOp[10]);
                        break;
                    case SSI_SET:                                 //�ɼ������ͼ������
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
* ��    �ƣ�__interrupt void usart1_tx (void)
* ��    �ܣ�RF�жϴ�����
* ��ڲ�������
* ���ڲ�������
* ��    ע����
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
    RF_UTXDIS;                  //���ܴ��ڷ���(RF)
  }
  
}
