#ifndef __APP_WDT_H
#define __APP_WDT_H

#include <includes.h>

typedef enum
{
    SWDT_STAT_IDLE,                             //������Ź�����
    SWDT_STAT_SUSPEN,                           //������Ź�����
    SWDT_STAT_RUN                               //������Ź�����
}SWDT_STAT;

typedef enum
{//���������������Ź�ID
    TSD_TASK_SWDT_ID,                           //���Ͳɼ�����
    MAX_SWDT_ID
}SWDT_ID;

typedef struct soft_wach_dog_timer
{
      u32        watchDogTimeOut;               //���Ź�������ʱ��ֵ
      u32        watchDogTime;                  //���Ź���ʱ��
      u8         watchDogCountBack;             //��һ�ο���ι��������
      u8         watchDogCount;                 //���Ź�ι��������
      SWDT_STAT  watchDogState;                 //���Ź���ʱ��״̬
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