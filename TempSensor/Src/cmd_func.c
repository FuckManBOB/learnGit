#include "cmd_func.h"

/****************************************************************************
*               以下CmdOp_命令都执行完毕后包括发送回应                      *
****************************************************************************/
/****************************************************************************
* 名    称：void CmdOp_RVN(void)
* 功    能：执行命令任务--读版本号(READ VERSION NUMBER)
* 入口参数：无
* 出口参数：无
* 备    注：无
****************************************************************************/
void CmdOp_RVN(void)
{
   memcpy(VsnFram.SlaveAddr, FA_SN_ADDR_START, 6);
   VsnFram.Version = FB_VERSION;
   
   RF_Send_Data((char *)&VsnFram, sizeof(VSN_FRAM));
}


/****************************************************************************
* 名    称：void CmdOp_WPTR(void)
* 功    能：执行命令任务--写参数（WRITE PARAMTER）
* 入口参数：无
* 出口参数：无
* 备    注：无
****************************************************************************/
void CmdOp_WPTR(void)
{
   /* 检查参数是否符合规定,符合写入flash,不符将参数返回发送 */
   if(BasicParameterCheck(RfBuf) == APP_ERR)                      //检查参数是否符合规定
   {
      /* 发送错误应答帧将FLASH存储区中的原参数发送回上位 */
      ParameterHandle(S_PARAMTER_DOWNLOAD_ERR);
   }
   else
   {
      /* 写入FLASH_B中 */
      FlashWrite(FLASH_SEG_B, (RfBuf + 10), RfBuf[9]);
      /* 发送正常应答帧 将写入FLASH后的参数发送回上位*/
      ParameterHandle(S_PARAMTER_DOWNLOAD_OK);
      /* 校准时间 */
      LocalTime = DateTime_Convert_Seconds(FB_YEAR, FB_MONTH, FB_DAY, 
                                            FB_HOUT, FB_MINITES, FB_SECONDS);      
   }    
}


/****************************************************************************
* 名    称：void CmdOp_RPTR(void)
* 功    能：执行命令任务--读参数（READ PARAMTER）
* 入口参数：无
* 出口参数：无
* 备    注：无
****************************************************************************/
void CmdOp_RPTR(void)
{
    /* 发送正常回应帧 将FLASH中的参数读取出来通过参数帧发送回上位*/
    ParameterHandle(S_READ_PARAMTER);
}


/****************************************************************************
* 名    称：void CmdOp_SSD(void)
* 功    能：执行命令任务--采集发送数据（sample send data）
* 入口参数：无
* 出口参数：无
* 备    注：无
****************************************************************************/
#if D_SAMPLE_TDATA
void CmdOp_SSD(void)
{
    /* 采集一次温度值 并将其和标准帧一起发送至上位 */
    FramHandle(CurTime, TemperatureSample().TempValue, Battery_Value_Get(), S_SAMPLE_TDATA, RESERVE_WORD);
    RF_Send_Data((char *)&StdFram, sizeof(STD_FRAM));
}
#endif
/****************************************************************************
* 名    称：void CmdOp_RRF(void)
* 功    能：执行命令任务--RTU回应帧（RTU RESPONSE FRAM）
* 入口参数：无
* 出口参数：无
* 备    注：无
****************************************************************************/
void CmdOp_RRF(void)
{
    
}

/****************************************************************************
* 名    称：void CmdOp_PTR(void)
* 功    能：执行命令任务--读SN号（READ SN NUMBER）
* 入口参数：无
* 出口参数：无
* 备    注：无
****************************************************************************/
void CmdOp_RSN(void)
{
    /* 发送正常回应帧 将FLASH中后的SN号读取出来通过SN号帧发送回上位*/
    SnNumberHandle(S_READ_SN_NUM_OK);
}


/****************************************************************************
* 名    称：void CmdOp_WDA(void)
* 功    能：执行命令任务--写SN号（WRITE SN NUMBER）
* 入口参数：无
* 出口参数：无
* 备    注：无
****************************************************************************/
void CmdOp_WSN(void)
{
  /* 消息正确的接受,将基本参数写入flash,配置参数 */
   if( SnCheck(RfBuf) == APP_ERR)                      //检查参数是否符合规定
   {
      /* 发送错误应答帧将FLASH存储区中的原参数发送回上位 */
      SnNumberHandle(S_WRITE_SN_NUM_ERR);  
   }
   else
   {  /* 发送正常应答帧 将写入FLASH后的参数发送回上位*/ 
      FlashWrite(FLASH_SEG_A, (RfBuf + 10), RfBuf[9]);
      SnNumberHandle(S_WRITE_SN_NUM_OK);
   }
}

/****************************************************************************
* 名    称：void CmdOp_CTC(void)
* 功    能：执行命令任务--当前时间校准（CURRENT TIME CHECK）
* 入口参数：无
* 出口参数：0 - 需重新设置时间 1 - 无需设置
* 备    注：无
****************************************************************************/
u8 CmdOp_CTC(void)
{
    u32 CurTimeTemp,CheckTime;
    char *CheckTimePtr = NULL;
    char tmp[4] ;
    
    CheckTimePtr = RfBuf + 10;                   //校验时间偏移地址   
    memcpy(tmp, CheckTimePtr, 4);
    CheckTime = *(u32 *)tmp;                     //校验时间
    CurTimeTemp = CurTime;                       //当前时间
    
    if( abs(CheckTime - CurTimeTemp) > 1 )      //误差1S以上重新设置时间
    {
      LocalTime = CheckTime + 2;
      return 0;
    }
    return 1;
}

/****************************************************************************
* 名    称：void CmdOp_RCT(void)
* 功    能：执行命令任务--读取当前时间（READ CURRENT TIME）
* 入口参数：无
* 出口参数：无
* 备    注：无
****************************************************************************/
void CmdOp_RCT(void)
{
    /* 将当前时间(秒)转换为年月日时分秒并按时间帧发送 */
    ReadCurTimeAndSendTimeFram(S_READ_CUR_TIME);
}


/****************************************************************************
* 名    称：void CmdOp_WS(void)
* 功    能：执行命令任务--预警设置（WARNING SET）
* 入口参数：无
* 出口参数：无
* 备    注：无
****************************************************************************/
void CmdOp_WS(void)
{
   char temp[15] = {0};
   
   /* 消息正确的接受,将基本参数写入flash,配置参数 */
   if( WarnSetCheck(RfBuf) == APP_ERR)                      //检查参数是否符合规定
   {
      /* 发送错误应答帧将FLASH存储区中的原参数发送回上位 */
      WarnSetHandle(S_WARNING_SET_ERR);  
   }
   else
   {  
      /* 将FLASH_B中原有数据copy出来，防止写入部分数据时，其他数据被擦除 */
      memcpy(temp, FB_PARAMTER_ADDR, 15);
      /* 将上下限预警写入临时数组的预警区域 */
      memcpy((temp + 2), (RfBuf + 10), 2);
      /* 写入FLASH_B中 */
      FlashWrite(FLASH_SEG_B, temp, 15);                  
      /* 发送正常应答帧 将写入FLASH后的参数发送回上位*/
      WarnSetHandle(S_WARNING_SET_OK);
   }
}

/****************************************************************************
* 名    称：void CmdOp_SIV(void)
* 功    能：执行命令任务--采集和发送间隔设置（SAMPLE && SEND INTERVIEW）
* 入口参数：无
* 出口参数：无
* 备    注：无
****************************************************************************/
void CmdOp_SSI(void)
{
  char temp[15] = {0};
   
   /* 消息正确的接受,将基本参数写入flash,配置参数 */
   if( SSICheck(RfBuf) == APP_ERR)                      //检查参数是否符合规定
   {
      /* 发送错误应答帧将FLASH存储区中的采集、发送间隔数据发送回上位 */
      SSISetHandle(S_SSI_SET_ERR);  
   }
   else
   {  
      /* 将FLASH_B中原有数据copy出来，防止写入部分数据时，其他数据被擦除 */
      memcpy(temp, FB_PARAMTER_ADDR, 15);
      /* 将上采集发送间隔设置数据写入临时数组的相应区域 */
      memcpy(temp, (RfBuf + 10), 2);
      /* 写入FLASH_B中 */
      FlashWrite(FLASH_SEG_B, temp, 15);                  
      /* 发送正常应答帧 将写入FLASH后的参数发送回上位*/
      SSISetHandle(S_SSI_SET_OK);
   }
}

/*---------------------------------------------------------------------------*/
/*                      以上是命令功能函数                                   */
/*                      以下是功能函数的子函数                               */
/*---------------------------------------------------------------------------*/




/****************************************************************************
* 名    称：void GetBasicParameter(void)
* 功    能：获取flash段B 0x1000地址处的设备参数
* 入口参数：无
* 出口参数：无
* 备    注：无
****************************************************************************/
void GetBasicParameter(void)
{
   char *ptrTemp  = NULL;

   ptrTemp = FLASH_SEGB_ADDR;                           //起始地址为B段0x1000
   Paramter_SampleTime = *ptrTemp;                      //采集间隔
   
   Paramter_SendTime = *((char *)(++ptrTemp));          //发送间隔
   
   Paramter_WarningUpperLimit = *((char *)(++ptrTemp)); //报警上限
   
   Paramter_WarningLowerLimit = *((char *)(++ptrTemp)); //报警下限
   
   Paramter_Year  = *((short *)(++ptrTemp));            //年
   
   Paramter_Month = *((char *)(++ptrTemp));             //月
   
   Paramter_Day   = *((char *)(++ptrTemp));             //日
   
   Paramter_Hour  = *((char *)(++ptrTemp));             //时
   
   Paramter_Min   = *((char *)(++ptrTemp));             //分
   
   Paramter_Sec   = *((char *)(++ptrTemp));             //秒
   
}
/****************************************************************************
* 名    称：u8 BasicParameterCheck(char *Ptr)
* 功    能：检查基本下载参数是否符合规定
* 入口参数：参数地址(参数下载帧内的参数数据域地址)
* 出口参数：1-OK  0-ERR
* 备    注：检查内容：1.采集间隔 1 ~ 59(Min)
*                     2.发送间隔 1 ~ 59(Min)
*                     3.报警上限 30 ~ 80（℃）
*                     4.报警下限 -35 ~ 0（℃）
*                     5.年  2010 ~ 2105 （年）
*                     6.月  1 ~ 12      （月）
*                     7.日  1 ~ 31      （日）
*                     8.时  0 ~ 24      （时）
*                     9.分  0 ~ 59      （分）
*                    10.秒  0 ~ 59      （秒）
*注意：在IAR中char不等同于signed char
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
   
   ptrTemp = Ptr + 10;                  //将地址指向参数起始地址
   
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
* 名    称：u8 BasicParameterCheck(char *Ptr)
* 功    能：检查SN号是否符合规定
* 入口参数：参数地址(参数下载帧内的参数数据域地址)
* 出口参数：1-OK  0-ERR
* 备    注：检查内容：1.年份 大于1970年
*                     2.设备类型 ：0x01 - 温度
*注意：在IAR中char不等同于signed char
****************************************************************************/
u8 SnCheck(char *SnPtr)
{
  SN_STRUCT   Sn_Struct;
  char *SnPtrTemp = NULL;
  
  
  SnPtrTemp = SnPtr + 10;                  //将地址指向SN数据地址
   
  Sn_Struct.SN_DevId   = *((u16 *)SnPtrTemp++);         //设备编号(2字节)
  SnPtrTemp++;
  Sn_Struct.SN_Batch  = *((u8 *)SnPtrTemp++);
  Sn_Struct.SN_Year    = *((u16 *)SnPtrTemp++);         //年(2字节)
  SnPtrTemp++;
  Sn_Struct.SN_DevType = *((u8*)SnPtrTemp); 
  
  if( (Sn_Struct.SN_Year >= 1970 ) && (Sn_Struct.SN_DevType == DEVICE_TYPE) )
    return 1;
  return 0;
}

/****************************************************************************
* 名    称：void CurTimeCheck(char *ctcPtr)
* 功    能：检验时间参数是否符合规定
* 入口参数：时间帧指针
* 出口参数：1-OK  0-ERR
* 备    注：无
****************************************************************************/
u8 CurTimeCheck(char *ctcPtr)
{
    char *ptrTemp    = NULL;
    u16  Year;
    signed char Month,Day,Hour,Min,Sec;
  
    ptrTemp = ctcPtr + 10;              //指向年月日时分秒指针
    
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
* 名    称：u8 WarnSetCheck(char *wscPtr)
* 功    能：预警设置是否符合规定
* 入口参数：预警帧指针
* 出口参数：1-OK  0-ERR
* 备    注：无
****************************************************************************/
u8 WarnSetCheck(char *wscPtr)
{
    char *ptrTemp = NULL;
    signed char WarnUL;
    signed char WarnLL;
    
    ptrTemp = wscPtr + 10;              //指向预警指针
    
    WarnUL  = *((signed char*)ptrTemp++);
    WarnLL  = *((signed char*)ptrTemp);
    if( (WarnUL >= 30 && WarnUL <= 80) && (WarnLL >= -35 && WarnLL <= 0) )
     return 1;
   return 0;     
}

/****************************************************************************
* 名    称：u8 SSICheck(char *ssiPtr)
* 功    能：采集和发射设置是否符合规定
* 入口参数：采集、发射帧指针
* 出口参数：1-OK  0-ERR
* 备    注：无
****************************************************************************/
u8 SSICheck(char *ssiPtr)
{
    char *ptrTemp = NULL;
    signed char SampleTime;
    signed char SendTime;
    
    ptrTemp = ssiPtr + 10;              //指向采集、发送间隔指针
    
    SampleTime  = *((signed char*)ptrTemp++);
    SendTime    = *((signed char*)ptrTemp);
    if( (SampleTime >= 1 && SampleTime <= 59) && (SendTime >= 1 && SendTime <= 59) )
     return 1;
   return 0;    
}

/****************************************************************************
* 名    称：void FramHandle(u32 CurTime_Sec, float SData, 
*           float BCData, u8 RespCmdWord, u32 Reserve)
* 功    能：标准帧赋值
* 入口参数：1.当前时间(秒)
*           2.所要发送的温度数据(发送的是之前采集的温度值，并非此刻温度值)
*           3.当前的电量值
*           4.保留值(赋0xFFFFFFFF值即可)
* 出口参数：无
* 备    注：无
****************************************************************************/
void FramHandle(u32 CurTime_Sec, float SData, float BCData, u8 RespCmdWord, u32 Reserve)
{
  static u32 CNT;
  u8 	 uCRC 	        = 0;				//校验初始值
  u32    i              = 0;
  char   *DataPtr       = NULL;
  
  SubFram.CycleNumber   = ++CNT;                        //周期号累加
  SubFram.Time          = CurTime_Sec;                  //当前时间(秒单位)
  SubFram.SampleData    = SData;                        //当前采集的温度值
  SubFram.BatteryCharge = BCData;                       //当前采集的电池电压值
  SubFram.Reserve       = Reserve;                      //保留值
  
  memcpy(StdFram.SlaveAddr, FA_SN_ADDR_START, 6);       //SN号
  StdFram.CmdWord       = RespCmdWord;                  //命令字
  StdFram.DataLength    = sizeof(DATA_FORMAT);          //数据长度
  StdFram.Data          = SubFram;                      //标准帧数据域嵌套
  DataPtr               = (char *)&StdFram.Data;        //数据结构体指针
  for(i = 1; i <= StdFram.DataLength; i++)
     uCRC ^= *DataPtr++;  
  StdFram.CheckSum      = uCRC;                         //bcc异或校验
}

/****************************************************************************
* 名    称：void ParameterHandle(u8 RespCmdWord)
* 功    能：将flashA存储区中的参数数据赋值给发送结构体并带
*           上相应功能命令帧并将结构体发送
* 入口参数：回应的命令字1.正常回应
*                       2.异常回应
* 出口参数：无
* 备    注：无
****************************************************************************/
void ParameterHandle(u8 RespCmdWord)
{
    memcpy(ParamterFram.SlaveAddr, FA_SN_ADDR_START, 6);
    ParamterFram.CmdWord = RespCmdWord;
    memcpy(ParamterFram.Paramter, FB_PARAMTER_ADDR,15);
    RF_Send_Data((char *)&ParamterFram, sizeof(PARAMTER_FRAM));
}

/****************************************************************************
* 名    称：void SnNumberHandle(u8 RespCmdWord)
* 功    能：将flashB存储区中的SN号数据赋值给发送结构体并带上
*           相应功能命令帧并将结构体发送
* 入口参数：回应的命令字
* 出口参数：无
* 备    注：无
****************************************************************************/
void SnNumberHandle(u8 RespCmdWord)
{
    memcpy(SnFram.SlaveAddr, FA_SN_ADDR_START, 6);
    SnFram.CmdWord = RespCmdWord;
    memcpy(SnFram.SnNumber, FA_SN_ADDR_START,6);
    RF_Send_Data((char *)&SnFram, sizeof(SN_FRAM));
}

/****************************************************************************
* 名    称：void TimerHandle(u8 RespCmdWord)
* 功    能：将flashB存储区中的日期数据赋值给发送结构体并带上相应
*           功能命令帧并将结构体发送
* 入口参数：回应的命令字
* 出口参数：无
* 备    注：无
****************************************************************************/
void TimerHandle(u8 RespCmdWord)
{
    memcpy(TimeFram.SlaveAddr, FA_SN_ADDR_START, 6);
    TimeFram.CmdWord = RespCmdWord;
    memcpy(TimeFram.CurTime, FB_DATE_ADDR, 7);
    RF_Send_Data((char *)&TimeFram, sizeof(TIME_FRAM));
}

/****************************************************************************
* 名    称：void ReadCurTimeAndSendTimeFram(u8 RespCmdWord)
* 功    能：将当前时间转换成年月日时分秒并设置命令字后按时间帧发送
* 入口参数：回应的命令字
* 出口参数：无
* 备    注：无
****************************************************************************/
void ReadCurTimeAndSendTimeFram(u8 RespCmdWord)
{
   /* 将当前时间(秒)转换为年月日时分秒 */
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
* 名    称：void WarnSetHandle(u8 RespCmdWord)
* 功    能：将flashB存储区中的预警数据赋值给发送结构体并带上相应
*           功能命令帧并将结构体发送
* 入口参数：回应的命令字
* 出口参数：无
* 备    注：无
****************************************************************************/
void WarnSetHandle(u8 RespCmdWord)
{
   memcpy(WarnFram.SlaveAddr, FA_SN_ADDR_START, 6);
   WarnFram.CmdWord = RespCmdWord;
   memcpy(WarnFram.WarnSet, FB_WARN_SET, 2);
   RF_Send_Data((char *)&WarnFram, sizeof(WARN_FRAM));
}

/****************************************************************************
* 名    称：void WarnSetHandle(u8 RespCmdWord)
* 功    能：将flashB存储区中的采集、发送数据赋值给发送结构体并带上相应
*           功能命令帧并将结构体发送
* 入口参数：回应的命令字
* 出口参数：无
* 备    注：无
****************************************************************************/
void SSISetHandle(u8 RespCmdWord)
{
   memcpy(SsiFram.SlaveAddr, FA_SN_ADDR_START, 6);
   SsiFram.CmdWord = RespCmdWord;
   memcpy(SsiFram.SSISet, FB_SSI_SET, 2);
   RF_Send_Data((char *)&SsiFram, sizeof(SSI_FRAM));
}