#ifndef __HAL_TIME_H
#define __HAL_TIME_H

#include "includes.h"


#define NORMAL                  1               //正常
#define ABNORMAL                !NORMAL         //异常
#define SYSTEMTICK_PERIOD_S     1               //更新时间自增1s
#define CALIBRATION_TIME_ERR    60              //校准误差范围XX秒

/* 时间结构体 */
typedef struct 
{
	vu8 hour;
	vu8 min;
	vu8 sec;			
	//公历日月年周
	vu16 w_year;
	vu8  w_month;
	vu8  w_date;
	vu8  week;		 
}_calendar_obj;

extern _calendar_obj calendar;
extern u16 SampleInterval;

void TimeA_config(void);
void RTC_init(void);
void Time_Update(void);
u32 DateTime_Convert_Seconds(u16 syear,u8 smon,u8 sday,u8 hour,u8 min,u8 sec);
u8 Time_Calibration(u32 CurrentTime, u32 CalibrationTime);
u8 DateTime_Get(u32 curTime);
u32 BasicSecond_Get(void);
void SetCurrentTime(u32 CurrentTime_Seconds);

#endif