#ifndef  __BATTERY_H
#define  __BATTERY_H

#include "includes.h"



#define BATTERY_POWER_ON       P5DIR|=BIT5;P5OUT&=~BIT5;         //监测电池电量开关：开
#define BATTERY_POWER_OFF      P5DIR|=BIT5;P5OUT|=BIT5;          //监测电池电量开关：关




float Battery_Value_Get(void);

#endif