#ifndef __DS18B20_H__
#define __DS18B20_H__ 

#include "includes.h"

#define TEMP_POWER_ON     P6DIR|=BIT5;P6OUT&=~BIT5         //温度传感器开关：开
#define TEMP_POWER_OFF    P6DIR|=BIT5;P6OUT|=BIT5          //温度传感器开关：关


//DS18B20 DATA 端口                     产品测试板
#define   DQ_OUT     P6DIR|=BIT6 
#define   DQ_IN      P6DIR&=~BIT6 
#define   DQ_H       P6OUT|=BIT6 
#define   DQ_L       P6OUT&=~BIT6
#define   DQ_DATE    (P6IN&BIT6)

//DS18B20 相关寄存器
#define ReadRom			0x33
#define MatchRom		0x55
#define SearchRom		0xf0
#define AlertSearch		0xec
#define CopyScratchpad		0x48
#define SkipRom			0xcc
#define ReadPower		0xb4

//DS18B20 相关命令
#define ConvertTemperature	0x44
#define ReadScratchpad		0xbe
#define WriteScratchpad		0x4e
#define RecallE			0xb8

#define NAGETIVE_FLAG            -1
#define POSITIVE_FLAG             1

#define TEMPERATURE_ERR          -1                     //出现85的错误现象
#define TEMPERATURE_OK            1                     //采集正常
typedef struct TempStruct{
char TempFlag;
float TempValue;
signed char Err;
}TEMP_STRUCT;


unsigned char DS18B20_Init(void);
unsigned char DS18B20_ReadBit(void);
void DS18B20_WriteBit(unsigned char Data);
unsigned char DS18B20_ReadByte(void);
void DS18B20_WriteByte(unsigned char Data);
TEMP_STRUCT DS18B20_ReadTemp(void);
TEMP_STRUCT TemperatureSample(void);
#endif
