#include "cmd_func.h"

/****************************************************************************
*               ����CmdOp_���ִ����Ϻ�������ͻ�Ӧ                      *
****************************************************************************/
/****************************************************************************
* ��    �ƣ�void CmdOp_RVN(void)
* ��    �ܣ�ִ����������--���汾��(READ VERSION NUMBER)
* ��ڲ�������
* ���ڲ�������
* ��    ע����
****************************************************************************/
void CmdOp_RVN(void)
{
   memcpy(VsnFram.SlaveAddr, FA_SN_ADDR_START, 6);
   VsnFram.Version = FB_VERSION;
   
   RF_Send_Data((char *)&VsnFram, sizeof(VSN_FRAM));
}


/****************************************************************************
* ��    �ƣ�void CmdOp_WPTR(void)
* ��    �ܣ�ִ����������--д������WRITE PARAMTER��
* ��ڲ�������
* ���ڲ�������
* ��    ע����
****************************************************************************/
void CmdOp_WPTR(void)
{
   /* �������Ƿ���Ϲ涨,����д��flash,�������������ط��� */
   if(BasicParameterCheck(RfBuf) == APP_ERR)                      //�������Ƿ���Ϲ涨
   {
      /* ���ʹ���Ӧ��֡��FLASH�洢���е�ԭ�������ͻ���λ */
      ParameterHandle(S_PARAMTER_DOWNLOAD_ERR);
   }
   else
   {
      /* д��FLASH_B�� */
      FlashWrite(FLASH_SEG_B, (RfBuf + 10), RfBuf[9]);
      /* ��������Ӧ��֡ ��д��FLASH��Ĳ������ͻ���λ*/
      ParameterHandle(S_PARAMTER_DOWNLOAD_OK);
      /* У׼ʱ�� */
      LocalTime = DateTime_Convert_Seconds(FB_YEAR, FB_MONTH, FB_DAY, 
                                            FB_HOUT, FB_MINITES, FB_SECONDS);      
   }    
}


/****************************************************************************
* ��    �ƣ�void CmdOp_RPTR(void)
* ��    �ܣ�ִ����������--��������READ PARAMTER��
* ��ڲ�������
* ���ڲ�������
* ��    ע����
****************************************************************************/
void CmdOp_RPTR(void)
{
    /* ����������Ӧ֡ ��FLASH�еĲ�����ȡ����ͨ������֡���ͻ���λ*/
    ParameterHandle(S_READ_PARAMTER);
}


/****************************************************************************
* ��    �ƣ�void CmdOp_SSD(void)
* ��    �ܣ�ִ����������--�ɼ��������ݣ�sample send data��
* ��ڲ�������
* ���ڲ�������
* ��    ע����
****************************************************************************/
#if D_SAMPLE_TDATA
void CmdOp_SSD(void)
{
    /* �ɼ�һ���¶�ֵ ������ͱ�׼֡һ��������λ */
    FramHandle(CurTime, TemperatureSample().TempValue, Battery_Value_Get(), S_SAMPLE_TDATA, RESERVE_WORD);
    RF_Send_Data((char *)&StdFram, sizeof(STD_FRAM));
}
#endif
/****************************************************************************
* ��    �ƣ�void CmdOp_RRF(void)
* ��    �ܣ�ִ����������--RTU��Ӧ֡��RTU RESPONSE FRAM��
* ��ڲ�������
* ���ڲ�������
* ��    ע����
****************************************************************************/
void CmdOp_RRF(void)
{
    
}

/****************************************************************************
* ��    �ƣ�void CmdOp_PTR(void)
* ��    �ܣ�ִ����������--��SN�ţ�READ SN NUMBER��
* ��ڲ�������
* ���ڲ�������
* ��    ע����
****************************************************************************/
void CmdOp_RSN(void)
{
    /* ����������Ӧ֡ ��FLASH�к��SN�Ŷ�ȡ����ͨ��SN��֡���ͻ���λ*/
    SnNumberHandle(S_READ_SN_NUM_OK);
}


/****************************************************************************
* ��    �ƣ�void CmdOp_WDA(void)
* ��    �ܣ�ִ����������--дSN�ţ�WRITE SN NUMBER��
* ��ڲ�������
* ���ڲ�������
* ��    ע����
****************************************************************************/
void CmdOp_WSN(void)
{
  /* ��Ϣ��ȷ�Ľ���,����������д��flash,���ò��� */
   if( SnCheck(RfBuf) == APP_ERR)                      //�������Ƿ���Ϲ涨
   {
      /* ���ʹ���Ӧ��֡��FLASH�洢���е�ԭ�������ͻ���λ */
      SnNumberHandle(S_WRITE_SN_NUM_ERR);  
   }
   else
   {  /* ��������Ӧ��֡ ��д��FLASH��Ĳ������ͻ���λ*/ 
      FlashWrite(FLASH_SEG_A, (RfBuf + 10), RfBuf[9]);
      SnNumberHandle(S_WRITE_SN_NUM_OK);
   }
}

/****************************************************************************
* ��    �ƣ�void CmdOp_CTC(void)
* ��    �ܣ�ִ����������--��ǰʱ��У׼��CURRENT TIME CHECK��
* ��ڲ�������
* ���ڲ�����0 - ����������ʱ�� 1 - ��������
* ��    ע����
****************************************************************************/
u8 CmdOp_CTC(void)
{
    u32 CurTimeTemp,CheckTime;
    char *CheckTimePtr = NULL;
    char tmp[4] ;
    
    CheckTimePtr = RfBuf + 10;                   //У��ʱ��ƫ�Ƶ�ַ   
    memcpy(tmp, CheckTimePtr, 4);
    CheckTime = *(u32 *)tmp;                     //У��ʱ��
    CurTimeTemp = CurTime;                       //��ǰʱ��
    
    if( abs(CheckTime - CurTimeTemp) > 1 )      //���1S������������ʱ��
    {
      LocalTime = CheckTime + 2;
      return 0;
    }
    return 1;
}

/****************************************************************************
* ��    �ƣ�void CmdOp_RCT(void)
* ��    �ܣ�ִ����������--��ȡ��ǰʱ�䣨READ CURRENT TIME��
* ��ڲ�������
* ���ڲ�������
* ��    ע����
****************************************************************************/
void CmdOp_RCT(void)
{
    /* ����ǰʱ��(��)ת��Ϊ������ʱ���벢��ʱ��֡���� */
    ReadCurTimeAndSendTimeFram(S_READ_CUR_TIME);
}


/****************************************************************************
* ��    �ƣ�void CmdOp_WS(void)
* ��    �ܣ�ִ����������--Ԥ�����ã�WARNING SET��
* ��ڲ�������
* ���ڲ�������
* ��    ע����
****************************************************************************/
void CmdOp_WS(void)
{
   char temp[15] = {0};
   
   /* ��Ϣ��ȷ�Ľ���,����������д��flash,���ò��� */
   if( WarnSetCheck(RfBuf) == APP_ERR)                      //�������Ƿ���Ϲ涨
   {
      /* ���ʹ���Ӧ��֡��FLASH�洢���е�ԭ�������ͻ���λ */
      WarnSetHandle(S_WARNING_SET_ERR);  
   }
   else
   {  
      /* ��FLASH_B��ԭ������copy��������ֹд�벿������ʱ���������ݱ����� */
      memcpy(temp, FB_PARAMTER_ADDR, 15);
      /* ��������Ԥ��д����ʱ�����Ԥ������ */
      memcpy((temp + 2), (RfBuf + 10), 2);
      /* д��FLASH_B�� */
      FlashWrite(FLASH_SEG_B, temp, 15);                  
      /* ��������Ӧ��֡ ��д��FLASH��Ĳ������ͻ���λ*/
      WarnSetHandle(S_WARNING_SET_OK);
   }
}

/****************************************************************************
* ��    �ƣ�void CmdOp_SIV(void)
* ��    �ܣ�ִ����������--�ɼ��ͷ��ͼ�����ã�SAMPLE && SEND INTERVIEW��
* ��ڲ�������
* ���ڲ�������
* ��    ע����
****************************************************************************/
void CmdOp_SSI(void)
{
  char temp[15] = {0};
   
   /* ��Ϣ��ȷ�Ľ���,����������д��flash,���ò��� */
   if( SSICheck(RfBuf) == APP_ERR)                      //�������Ƿ���Ϲ涨
   {
      /* ���ʹ���Ӧ��֡��FLASH�洢���еĲɼ������ͼ�����ݷ��ͻ���λ */
      SSISetHandle(S_SSI_SET_ERR);  
   }
   else
   {  
      /* ��FLASH_B��ԭ������copy��������ֹд�벿������ʱ���������ݱ����� */
      memcpy(temp, FB_PARAMTER_ADDR, 15);
      /* ���ϲɼ����ͼ����������д����ʱ�������Ӧ���� */
      memcpy(temp, (RfBuf + 10), 2);
      /* д��FLASH_B�� */
      FlashWrite(FLASH_SEG_B, temp, 15);                  
      /* ��������Ӧ��֡ ��д��FLASH��Ĳ������ͻ���λ*/
      SSISetHandle(S_SSI_SET_OK);
   }
}

/*---------------------------------------------------------------------------*/
/*                      ����������ܺ���                                   */
/*                      �����ǹ��ܺ������Ӻ���                               */
/*---------------------------------------------------------------------------*/




/****************************************************************************
* ��    �ƣ�void GetBasicParameter(void)
* ��    �ܣ���ȡflash��B 0x1000��ַ�����豸����
* ��ڲ�������
* ���ڲ�������
* ��    ע����
****************************************************************************/
void GetBasicParameter(void)
{
   char *ptrTemp  = NULL;

   ptrTemp = FLASH_SEGB_ADDR;                           //��ʼ��ַΪB��0x1000
   Paramter_SampleTime = *ptrTemp;                      //�ɼ����
   
   Paramter_SendTime = *((char *)(++ptrTemp));          //���ͼ��
   
   Paramter_WarningUpperLimit = *((char *)(++ptrTemp)); //��������
   
   Paramter_WarningLowerLimit = *((char *)(++ptrTemp)); //��������
   
   Paramter_Year  = *((short *)(++ptrTemp));            //��
   
   Paramter_Month = *((char *)(++ptrTemp));             //��
   
   Paramter_Day   = *((char *)(++ptrTemp));             //��
   
   Paramter_Hour  = *((char *)(++ptrTemp));             //ʱ
   
   Paramter_Min   = *((char *)(++ptrTemp));             //��
   
   Paramter_Sec   = *((char *)(++ptrTemp));             //��
   
}
/****************************************************************************
* ��    �ƣ�u8 BasicParameterCheck(char *Ptr)
* ��    �ܣ����������ز����Ƿ���Ϲ涨
* ��ڲ�����������ַ(��������֡�ڵĲ����������ַ)
* ���ڲ�����1-OK  0-ERR
* ��    ע��������ݣ�1.�ɼ���� 1 ~ 59(Min)
*                     2.���ͼ�� 1 ~ 59(Min)
*                     3.�������� 30 ~ 80���棩
*                     4.�������� -35 ~ 0���棩
*                     5.��  2010 ~ 2105 ���꣩
*                     6.��  1 ~ 12      ���£�
*                     7.��  1 ~ 31      ���գ�
*                     8.ʱ  0 ~ 24      ��ʱ��
*                     9.��  0 ~ 59      ���֣�
*                    10.��  0 ~ 59      ���룩
*ע�⣺��IAR��char����ͬ��signed char
****************************************************************************/
u8 BasicParameterCheck(char *Ptr)
{
   char *ptrTemp    = NULL;
   signed char SampleTime;
   signed char SendTime;
   signed char WarnUL;
   signed char WarnLL;
   u16  Year;
   signed char Month,Day,Hour,Min,Sec;
   
   ptrTemp = Ptr + 10;                  //����ַָ�������ʼ��ַ
   
   SampleTime = *ptrTemp++;
   SendTime   = *ptrTemp++;
   WarnUL     = *ptrTemp++;
   WarnLL     = *ptrTemp++;
   Year       = *((u16 *)ptrTemp++);
   ptrTemp++;
   Month      = *ptrTemp++;
   Day        = *ptrTemp++;
   Hour       = *ptrTemp++;
   Min        = *ptrTemp++;
   Sec        = *ptrTemp;
   
   if( (SampleTime >= 1 && SampleTime <= 59) && (SendTime >= 1 && SendTime <= 59)
      && (WarnUL >= 30 && WarnUL <= 80) && (WarnLL >= 0 && WarnLL <= 40) &&
      (Year >= 2010 && Year <= 2105) && (Month >= 1 && Month <= 12) && 
      (Day >= 1 && Day <= 31) && (Hour >= 0 && Hour <= 24) && (Min >= 0 && Min <= 59)
       && (Sec >= 0 && Sec <= 59) )
     return 1;
   return 0;   
}

/****************************************************************************
* ��    �ƣ�u8 BasicParameterCheck(char *Ptr)
* ��    �ܣ����SN���Ƿ���Ϲ涨
* ��ڲ�����������ַ(��������֡�ڵĲ����������ַ)
* ���ڲ�����1-OK  0-ERR
* ��    ע��������ݣ�1.��� ����1970��
*                     2.�豸���� ��0x01 - �¶�
*ע�⣺��IAR��char����ͬ��signed char
****************************************************************************/
u8 SnCheck(char *SnPtr)
{
  SN_STRUCT   Sn_Struct;
  char *SnPtrTemp = NULL;
  
  
  SnPtrTemp = SnPtr + 10;                  //����ַָ��SN���ݵ�ַ
   
  Sn_Struct.SN_DevId   = *((u16 *)SnPtrTemp++);         //�豸���(2�ֽ�)
  SnPtrTemp++;
  Sn_Struct.SN_Batch  = *((u8 *)SnPtrTemp++);
  Sn_Struct.SN_Year    = *((u16 *)SnPtrTemp++);         //��(2�ֽ�)
  SnPtrTemp++;
  Sn_Struct.SN_DevType = *((u8*)SnPtrTemp); 
  
  if( (Sn_Struct.SN_Year >= 1970 ) && (Sn_Struct.SN_DevType == DEVICE_TYPE) )
    return 1;
  return 0;
}

/****************************************************************************
* ��    �ƣ�void CurTimeCheck(char *ctcPtr)
* ��    �ܣ�����ʱ������Ƿ���Ϲ涨
* ��ڲ�����ʱ��ָ֡��
* ���ڲ�����1-OK  0-ERR
* ��    ע����
****************************************************************************/
u8 CurTimeCheck(char *ctcPtr)
{
    char *ptrTemp    = NULL;
    u16  Year;
    signed char Month,Day,Hour,Min,Sec;
  
    ptrTemp = ctcPtr + 10;              //ָ��������ʱ����ָ��
    
    Year       = *((u16 *)ptrTemp++);
    ptrTemp++;
    Month      = *ptrTemp++;
    Day        = *ptrTemp++;
    Hour       = *ptrTemp++;
    Min        = *ptrTemp++;
    Sec        = *ptrTemp;
  
    if( (Year >= 2010 && Year <= 2105) && (Month >= 1 && Month <= 12) && 
      (Day >= 1 && Day <= 31) && (Hour >= 0 && Hour <= 24) && (Min >= 0 && Min <= 59)
       && (Sec >= 0 && Sec <= 59) )
     return 1;
   return 0;     
}

/****************************************************************************
* ��    �ƣ�u8 WarnSetCheck(char *wscPtr)
* ��    �ܣ�Ԥ�������Ƿ���Ϲ涨
* ��ڲ�����Ԥ��ָ֡��
* ���ڲ�����1-OK  0-ERR
* ��    ע����
****************************************************************************/
u8 WarnSetCheck(char *wscPtr)
{
    char *ptrTemp = NULL;
    signed char WarnUL;
    signed char WarnLL;
    
    ptrTemp = wscPtr + 10;              //ָ��Ԥ��ָ��
    
    WarnUL  = *((signed char*)ptrTemp++);
    WarnLL  = *((signed char*)ptrTemp);
    if( (WarnUL >= 30 && WarnUL <= 80) && (WarnLL >= -35 && WarnLL <= 0) )
     return 1;
   return 0;     
}

/****************************************************************************
* ��    �ƣ�u8 SSICheck(char *ssiPtr)
* ��    �ܣ��ɼ��ͷ��������Ƿ���Ϲ涨
* ��ڲ������ɼ�������ָ֡��
* ���ڲ�����1-OK  0-ERR
* ��    ע����
****************************************************************************/
u8 SSICheck(char *ssiPtr)
{
    char *ptrTemp = NULL;
    signed char SampleTime;
    signed char SendTime;
    
    ptrTemp = ssiPtr + 10;              //ָ��ɼ������ͼ��ָ��
    
    SampleTime  = *((signed char*)ptrTemp++);
    SendTime    = *((signed char*)ptrTemp);
    if( (SampleTime >= 1 && SampleTime <= 59) && (SendTime >= 1 && SendTime <= 59) )
     return 1;
   return 0;    
}

/****************************************************************************
* ��    �ƣ�void FramHandle(u32 CurTime_Sec, float SData, 
*           float BCData, u8 RespCmdWord, u32 Reserve)
* ��    �ܣ���׼֡��ֵ
* ��ڲ�����1.��ǰʱ��(��)
*           2.��Ҫ���͵��¶�����(���͵���֮ǰ�ɼ����¶�ֵ�����Ǵ˿��¶�ֵ)
*           3.��ǰ�ĵ���ֵ
*           4.����ֵ(��0xFFFFFFFFֵ����)
* ���ڲ�������
* ��    ע����
****************************************************************************/
void FramHandle(u32 CurTime_Sec, float SData, float BCData, u8 RespCmdWord, u32 Reserve)
{
  static u32 CNT;
  u8 	 uCRC 	        = 0;				//У���ʼֵ
  u32    i              = 0;
  char   *DataPtr       = NULL;
  
  SubFram.CycleNumber   = ++CNT;                        //���ں��ۼ�
  SubFram.Time          = CurTime_Sec;                  //��ǰʱ��(�뵥λ)
  SubFram.SampleData    = SData;                        //��ǰ�ɼ����¶�ֵ
  SubFram.BatteryCharge = BCData;                       //��ǰ�ɼ��ĵ�ص�ѹֵ
  SubFram.Reserve       = Reserve;                      //����ֵ
  
  memcpy(StdFram.SlaveAddr, FA_SN_ADDR_START, 6);       //SN��
  StdFram.CmdWord       = RespCmdWord;                  //������
  StdFram.DataLength    = sizeof(DATA_FORMAT);          //���ݳ���
  StdFram.Data          = SubFram;                      //��׼֡������Ƕ��
  DataPtr               = (char *)&StdFram.Data;        //���ݽṹ��ָ��
  for(i = 1; i <= StdFram.DataLength; i++)
     uCRC ^= *DataPtr++;  
  StdFram.CheckSum      = uCRC;                         //bcc���У��
}

/****************************************************************************
* ��    �ƣ�void ParameterHandle(u8 RespCmdWord)
* ��    �ܣ���flashA�洢���еĲ������ݸ�ֵ�����ͽṹ�岢��
*           ����Ӧ��������֡�����ṹ�巢��
* ��ڲ�������Ӧ��������1.������Ӧ
*                       2.�쳣��Ӧ
* ���ڲ�������
* ��    ע����
****************************************************************************/
void ParameterHandle(u8 RespCmdWord)
{
    memcpy(ParamterFram.SlaveAddr, FA_SN_ADDR_START, 6);
    ParamterFram.CmdWord = RespCmdWord;
    memcpy(ParamterFram.Paramter, FB_PARAMTER_ADDR,15);
    RF_Send_Data((char *)&ParamterFram, sizeof(PARAMTER_FRAM));
}

/****************************************************************************
* ��    �ƣ�void SnNumberHandle(u8 RespCmdWord)
* ��    �ܣ���flashB�洢���е�SN�����ݸ�ֵ�����ͽṹ�岢����
*           ��Ӧ��������֡�����ṹ�巢��
* ��ڲ�������Ӧ��������
* ���ڲ�������
* ��    ע����
****************************************************************************/
void SnNumberHandle(u8 RespCmdWord)
{
    memcpy(SnFram.SlaveAddr, FA_SN_ADDR_START, 6);
    SnFram.CmdWord = RespCmdWord;
    memcpy(SnFram.SnNumber, FA_SN_ADDR_START,6);
    RF_Send_Data((char *)&SnFram, sizeof(SN_FRAM));
}

/****************************************************************************
* ��    �ƣ�void TimerHandle(u8 RespCmdWord)
* ��    �ܣ���flashB�洢���е��������ݸ�ֵ�����ͽṹ�岢������Ӧ
*           ��������֡�����ṹ�巢��
* ��ڲ�������Ӧ��������
* ���ڲ�������
* ��    ע����
****************************************************************************/
void TimerHandle(u8 RespCmdWord)
{
    memcpy(TimeFram.SlaveAddr, FA_SN_ADDR_START, 6);
    TimeFram.CmdWord = RespCmdWord;
    memcpy(TimeFram.CurTime, FB_DATE_ADDR, 7);
    RF_Send_Data((char *)&TimeFram, sizeof(TIME_FRAM));
}

/****************************************************************************
* ��    �ƣ�void ReadCurTimeAndSendTimeFram(u8 RespCmdWord)
* ��    �ܣ�����ǰʱ��ת����������ʱ���벢���������ֺ�ʱ��֡����
* ��ڲ�������Ӧ��������
* ���ڲ�������
* ��    ע����
****************************************************************************/
void ReadCurTimeAndSendTimeFram(u8 RespCmdWord)
{
   /* ����ǰʱ��(��)ת��Ϊ������ʱ���� */
   DateTime_Get(CurTime);
   memcpy(TimeFram.CurTime, (char *)&calendar.w_year, 2);
   TimeFram.CurTime[2] = calendar.w_month;
   TimeFram.CurTime[3] = calendar.w_date;
   TimeFram.CurTime[4] = calendar.hour;
   TimeFram.CurTime[5] = calendar.min;
   TimeFram.CurTime[6] = calendar.sec;
   memcpy(TimeFram.SlaveAddr, FA_SN_ADDR_START, 6);//
   TimeFram.CmdWord = RespCmdWord;
   
   RF_Send_Data((char *)&TimeFram, sizeof(TIME_FRAM));
}

/****************************************************************************
* ��    �ƣ�void WarnSetHandle(u8 RespCmdWord)
* ��    �ܣ���flashB�洢���е�Ԥ�����ݸ�ֵ�����ͽṹ�岢������Ӧ
*           ��������֡�����ṹ�巢��
* ��ڲ�������Ӧ��������
* ���ڲ�������
* ��    ע����
****************************************************************************/
void WarnSetHandle(u8 RespCmdWord)
{
   memcpy(WarnFram.SlaveAddr, FA_SN_ADDR_START, 6);
   WarnFram.CmdWord = RespCmdWord;
   memcpy(WarnFram.WarnSet, FB_WARN_SET, 2);
   RF_Send_Data((char *)&WarnFram, sizeof(WARN_FRAM));
}

/****************************************************************************
* ��    �ƣ�void WarnSetHandle(u8 RespCmdWord)
* ��    �ܣ���flashB�洢���еĲɼ����������ݸ�ֵ�����ͽṹ�岢������Ӧ
*           ��������֡�����ṹ�巢��
* ��ڲ�������Ӧ��������
* ���ڲ�������
* ��    ע����
****************************************************************************/
void SSISetHandle(u8 RespCmdWord)
{
   memcpy(SsiFram.SlaveAddr, FA_SN_ADDR_START, 6);
   SsiFram.CmdWord = RespCmdWord;
   memcpy(SsiFram.SSISet, FB_SSI_SET, 2);
   RF_Send_Data((char *)&SsiFram, sizeof(SSI_FRAM));
}