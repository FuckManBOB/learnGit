#include "hal_time.h"

volatile u32 LocalTime = 0; /* this variable is used to create a time reference incremented by 1s */

/* 一年中的月份按天数排列 */
const u8 mon_table[12]={31,28,31,30,31,30,31,31,30,31,30,31};


_calendar_obj calendar;


/****************************************************************************
* 名    称：u8 Is_Leap_Year(u16 year)
* 功    能：判断闰年
* 入口参数：年
* 出口参数：1、闰年返回1	2、平年返回0
* 备	注：
****************************************************************************/
u8 Is_Leap_Year(u16 year)
{			  
    if(year%4==0) //必须能被4整除
    { 
      if(year%100==0) 
      { 
          if(year%400==0)
               return 1;//如果以00结尾,还要能被400整除 	   
          else 
               return 0;   
      }
      else 
          return 1;   
    }
    else 
        return 0;	
}

/****************************************************************************************
* 名    称：u32 DateTime_Convert_Seconds(u16 syear,u8 smon,u8 sday,u8 hour,u8 min,u8 sec)
* 功    能：将输入的日期转换成秒
* 入口参数：年、月、日、时、分、秒
* 出口参数：1、超过指定年份返回错误0	2、正确则返回转换后的秒数
* 备	注：入参年份条件  1970 < syear < 2099 是以1970年1月1日作为时钟基数
*****************************************************************************************/
u32 DateTime_Convert_Seconds(u16 syear,u8 smon,u8 sday,u8 hour,u8 min,u8 sec)
{
    u16 t;
    u32 seccount=0;

    if(syear < 1970 || syear > 2099)
            return 0;
                   
    for(t = 1970; t < syear; t++) 		//把所有年份的秒钟相加												 
    {
      if(Is_Leap_Year(t))
            seccount+=31622400;		  	//闰年的秒钟数
      else 
            seccount+=31536000;       		//平年的秒钟数
    }

    smon-=1;

    for(t = 0; t < smon; t++)        		//把前面月份的秒钟数相加
    {
      seccount += (u32)mon_table[t]*86400;	//月份秒钟数相加
      if(Is_Leap_Year(syear) && t == 1)
            seccount += 86400;			//闰年2月份增加一天的秒钟数          
    }
    seccount += (u32)(sday-1)*86400;		//把前面日期的秒钟数相加 
    seccount += (u32)hour*3600;			//小时秒钟数
    seccount += (u32)min*60;       		//分钟秒钟数
    seccount += sec;				//最后的秒钟加上去  
    return seccount;
}

/****************************************************************************************
* 名    称：u8 Time_Calibration(u32 CurrentTime, u32 CalibrationTime)
* 功    能：将当前日期的秒数与校准日期秒数进行比较，在一定范围内表示正常，超出设定范围为异常
* 入口参数：1、当前日期描述         2、校准的日期秒数
* 出口参数：1、在正常范围内则返回1	2、超出范围则返回0表示异常
* 备	注：入参条件是以1970年1月1日作为时钟基数
*****************************************************************************************/
u8 Time_Calibration(u32 CurrentTime, u32 CalibrationTime)
{
  u32 Temp;
  Temp = abs(CurrentTime - CalibrationTime);
  if(Temp < CALIBRATION_TIME_ERR)return NORMAL;
  else
    return ABNORMAL;
}

/****************************************************************************
* 名    称：u8 DateTime_Get(void)
* 功    能：获取当前的时间
* 入口参数：无
* 出口参数：正常0 错误1
* 备	注：
****************************************************************************/
u8 DateTime_Get(u32 curTime)
{
    static u16 daycnt = 0;
    u32 timecount = 0; 
    u32 temp = 0;		//计天数
    u16 temp1 = 0;	        //计年份	  
    timecount = curTime;        //得到计数器中的值(秒钟数)			 

    temp = timecount/86400;   //得到天数(秒钟数对应的)
    if(daycnt != temp)//超过一天了
    {	  
        daycnt = temp;
        temp1 = 1970;	//从1970年开始
        while(temp >= 365)
        {				 
        if(Is_Leap_Year(temp1))//是闰年
        {
            if(temp >= 366)
                    temp -= 366;//闰年的秒钟数
            else 
                    break;  
        }
        else 
            temp -= 365;	  //平年 
        temp1++;  
        }   
        calendar.w_year = temp1;//得到年份

        temp1 = 0;

        while(temp >= 28)//超过了一个月
        {
        if(Is_Leap_Year(calendar.w_year)&&temp1 == 1)//当年是不是闰年/2月份
        {
            if(temp >= 29)
                    temp -= 29;//闰年的秒钟数
            else 
                    break; 
        }
        else 
        {
            if(temp >= mon_table[temp1])
                    temp -= mon_table[temp1];//平年
            else 
                    break;
        }

        temp1++;
          
        }
        calendar.w_month = temp1+1;	//得到月份
        calendar.w_date  = temp+1;  	//得到日期 
    }
    temp = timecount%86400;     	//得到秒钟数   	   
    calendar.hour = temp/3600;     	//小时
    calendar.min  = (temp%3600)/60; 	//分钟	
    calendar.sec  = (temp%3600)%60; 	//秒钟   
    return 0;
}	 

/****************************************************************************
* 名    称：u32 BasicSecond_Get(void)
* 功    能：获取从1970年起当前的秒数
* 入口参数：无
* 出口参数：返回当前秒数
* 备	注：
****************************************************************************/	 
u32 BasicSecond_Get(void)
{
    return LocalTime;
}

/****************************************************************************
* 名    称：void SetCurrentTime(u32 CurrentTime_Seconds)
* 功    能：设置当前时间
* 入口参数：当前时间换算后的秒数（从1970年起当前的秒数）
* 出口参数：无
* 备	注：
****************************************************************************/
void SetCurrentTime(u32 CurrentTime_Seconds)
{
    LocalTime = CurrentTime_Seconds;
}
