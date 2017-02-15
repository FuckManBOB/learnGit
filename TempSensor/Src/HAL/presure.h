#ifndef  __PRESURE_H
#define  __PRESURE_H

//#define BATTERY_POWER_OFF    P4DIR|=BIT2;P4OUT&=~BIT2        //监测电池电量开关：关
//#define BATTERY_POWER_ON     P4DIR|=BIT2;P4OUT|=BIT2         //监测电池电量开关：开

void Presure_Value_Get(void);


#endif