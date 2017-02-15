#ifndef __APP_WDT_H
#define __APP_WDT_H

#include <includes.h>

typedef enum
{
    SWDT_STAT_IDLE,                             //软件看门狗空闲
    SWDT_STAT_SUSPEN,                           //软件看门狗挂起
    SWDT_STAT_RUN                               //软件看门狗运行
}SWDT_STAT;

typedef enum
{//往这里添加软件看门狗ID
    TSD_TASK_SWDT_ID,                           //发送采集任务
    MAX_SWDT_ID
}SWDT_ID;

typedef struct soft_wach_dog_timer
{
      u32        watchDogTimeOut;               //看门狗计数超时初值
      u32        watchDogTime;                  //看门狗定时器
      u8         watchDogCountBack;             //上一次看门喂狗计数器
      u8         watchDogCount;                 //看门狗喂狗计数器
      SWDT_STAT  watchDogState;                 //看门狗定时器状态
      u8         NOUSE8;
}SOFT_WATCH_DOG_TIMER;


extern SOFT_WATCH_DOG_TIMER SoftWatchDogTimerList[MAX_SWDT_ID];
extern u8 StopWDTFedMake;

void SoftWdtISR(void);
void HardIWDTInit(void);
void IWDT_Feed(void);
void HardWDTInit(void);
u8 SoftWdtInit(SWDT_ID SwdtId, u32 TimerTop);
u32 MsToOSTicks(u32 CommanTime);
void SoftWdtFed(SWDT_ID SwdtId);




#endif