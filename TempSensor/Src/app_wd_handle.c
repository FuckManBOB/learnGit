#include "app_wd_handle.h"

#define SAMPLE_FAIL_TIMES         3             //采集失败次数 

extern _calendar_obj calendar;
extern OS_EVENT *WdHandleMbox;         //温度采集处理邮箱

void LcdDisplay(TEMP_STRUCT Temp_Value);
TEMP_STRUCT SampleTemperature(u8 sampleFailCnt);



/****************************************************************************
* 名    称：void WdHandle(u32 CycleNum, u32 SampleTime)
* 功    能：温度采集、电量采集、显示、发送数据
* 入口参数：1.周期编号 2.采集时间
* 出口参数：无
* 备    注：
****************************************************************************/
void WdHandle(u32 CycleNum)
{
      TEMP_STRUCT  Temp_Value;                       //温度结构体变量
      float        BC_Data;                          //电池电量变量
      
      /*1.采集温度*/
      Temp_Value = SampleTemperature(SAMPLE_FAIL_TIMES);
      
      /*2.获取电池电量*/
      BC_Data = Battery_Value_Get();
      
      /*3.液晶显示*/
      LcdDisplay(Temp_Value);
      
      /*4.发送数据*/
      F8913D_N_SLEEP_RQ;                             //射频设备唤醒请求，准备发送数据
      OSTimeDlyHMSM(0,0,0,500);
      FramHandle(CurTime, Temp_Value.TempValue, BC_Data, CMD_WORD_TS, RESERVE_WORD);         //处理温度包
      RF_Send_Data((char *)&StdFram, sizeof(STD_FRAM));                                      //发送数据
      OSTimeDlyHMSM(0,0,2,0);                      //等待发送完再睡(原因是中断发送方式)
      F8913D_SLEEP_RQ;                             //数据发送完，设备休眠请求
}

/****************************************************************************
* 名    称：TEMP_STRUCT SampleTemperature(u8 sampleFailCnt)
* 功    能：采集温度
* 入口参数：最多连续采集失败次数
* 出口参数：温度结构体
* 备    注：在限定失败次数内有一次采集成功则返回采集结果
****************************************************************************/
TEMP_STRUCT SampleTemperature(u8 sampleFailCnt)
{
      u8  S_SampleErrCnt;                           //采集错误次数
      TEMP_STRUCT Temp_Value = {0, 0.0, 0};         //温度值包括温度和正负标志
      
      for(S_SampleErrCnt = sampleFailCnt; S_SampleErrCnt > 0; S_SampleErrCnt--)
      {
        Temp_Value  = TemperatureSample();          //获取温度
        if(Temp_Value.Err != TEMPERATURE_ERR)       //采集未出错则退出采集
          break;
      }
      return Temp_Value;
}

/****************************************************************************
* 名    称：void LcdDisplay(char tempFlag)
* 功    能：LCD液晶显示
* 入口参数：温度正负标志
* 出口参数：无
* 备    注：无
****************************************************************************/
void LcdDisplay(TEMP_STRUCT Temp_Value)
{
    int temp = 0;
    if( POSITIVE_FLAG == Temp_Value.TempFlag )
    {
      LcdClean();                                       //清屏
      temp = (int)(Temp_Value.TempValue * DECIMALS);    //将浮点乘以10倍(一个小数)后强转成Int
      /* 显示正温度 */
      LcdPosNumber( temp );
      WriteAllData(GEWEI,0x00);                         //将异常的个位清零
    }
    else
    {
      LcdClean();                                       //清屏
      if( Temp_Value.TempValue < -0.0001 && Temp_Value.TempValue > -0.1 )     //解决-0.0001在液晶无法显示完全而造成的误会
        temp = (int)(-0.1 * DECIMALS);                  //将-0.0001 ~ -0.1区间内的值都认为是-0.1便于液晶显示该区间的负温度不至于误会为-00.0
      else
        temp = (int)(Temp_Value.TempValue * DECIMALS);  //将浮点乘以10倍(一个小数)后强转成Int
      /* 显示负温度 */
      LcdNegNumber( abs(temp) );                        //将负值转换取正给LCD驱动处理
      WriteAllData(GEWEI,0x00);                         //将异常的个位清零
    }
    
    /* 电量和信号监测显示 */
    Symbol_Lcd_Display();
}

/****************************************************************************
* 名    称：void IsSendTime(u32 curSeconds, u8 timePoint)
* 功    能：设置在某一时刻激活温度设备进行采集工作
* 入口参数：1.当前时间秒数 2.索要设定的唤醒时刻(如每时刻起的第15分钟)
* 出口参数：成功：0失败：1
* 备    注：无
****************************************************************************/
int IsSendTime(u32 curSeconds, u8 timePoint)
{
  static char rcdCurMin = -1;             //记录当前分钟数
  
  if(DateTime_Get(curSeconds) != 0)
    return 1;                   //错误
  
  
  if((calendar.min % timePoint) == 0)
  {
     if(rcdCurMin != calendar.min)
     {
        rcdCurMin = calendar.min;     //记录当前分钟数，用来下面区分分钟有没有变化
        //每个小时起始每个timePoint分钟向Wdhandle中的采集发送一次信号
        OSMboxPost(WdHandleMbox, (void *)1);
        return 0;
     }
  }
  return 1;
}