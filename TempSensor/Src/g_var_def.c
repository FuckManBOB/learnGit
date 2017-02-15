#include "includes.h"


u8  CurMode   = 0;                        //当前模式：0-命令模式 1-透传模式

u8  RfMsg     = 1;                        //射频睡眠邮箱消息：用来传递消息值

u8  SoftWatchDogOverTime;                 //软件看门狗溢出标志

char CmdModeBuffer[CMD_MODE_BUFFER_LENGTH];//命令模式缓存
u8   CmdModeBufferIndex; 

char RfBuf[RF_BUFFER_LENGTH];             //透传模式缓存
s16  RfIndex;

/* Config 相关全局变量 */
char ConfigBuf[CONFIG_BUFFER_LENGTH];	//用于接收配置信息
s16  ConfigBufIndex; 		        //用于接收配置信息计数


/*
*********************************************************************************************************
*                                            基本参数
*********************************************************************************************************
*/
volatile char  Paramter_SampleTime;                        //采集间隔
volatile char  Paramter_SendTime;                          //发送间隔
volatile char  Paramter_WarningUpperLimit;                 //报警上限
volatile char  Paramter_WarningLowerLimit;                 //报警下限
volatile u16   Paramter_Year;                              //年
volatile char  Paramter_Month;                             //月
volatile char  Paramter_Day;                               //日
volatile char  Paramter_Hour;                              //时
volatile char  Paramter_Min;                               //分
volatile char  Paramter_Sec;                               //秒

u8  Paramter_Sn[6];                                      //SN号

DATA_FORMAT     SubFram         = {1, 1, 1, 0, 12, 0, 0, 0};
      

STD_FRAM        StdFram         = {{0xFE, 0xFE, 0xFE}, 0x68, {0xFF, 0xFF, 0xFF, 0xFF,
                                    0xFF, 0xFF}, 0x68, 0x00, 0x00, {0x00}, 0xFF, 0x16};

VSN_FRAM        VsnFram         = {{0xFE, 0xFE, 0xFE}, 0x68, {0xFF, 0xFF, 0xFF, 0xFF, 
                                    0xFF, 0xFF}, 0x68, 0x80, 0x02, 0, 0xFF, 0x16};

PARAMTER_FRAM   ParamterFram    = {{0xFE, 0xFE, 0xFE}, 0x68, {0xFF, 0xFF, 0xFF, 0xFF, 
                                    0xFF, 0xFF}, 0x68, 0, 0x0F, {0}, 0xFF, 0x16};

SN_FRAM         SnFram          = {{0xFE, 0xFE, 0xFE}, 0x68, {0xFF, 0xFF, 0xFF, 0xFF, 
                                    0xFF, 0xFF}, 0x68, 0, 0x06, {0}, 0xFF, 0x16};

TIME_FRAM       TimeFram        = {{0xFE, 0xFE, 0xFE}, 0x68, {0xFF, 0xFF, 0xFF, 0xFF, 
                                    0xFF, 0xFF}, 0x68, 0, 0x07, {0}, 0xFF, 0x16};

WARN_FRAM       WarnFram        = {{0xFE, 0xFE, 0xFE}, 0x68, {0xFF, 0xFF, 0xFF, 0xFF, 
                                    0xFF, 0xFF}, 0x68, 0, 0x02, {0}, 0xFF, 0x16};

SSI_FRAM        SsiFram         = {{0xFE, 0xFE, 0xFE}, 0x68, {0xFF, 0xFF, 0xFF, 0xFF, 
                                    0xFF, 0xFF}, 0x68, 0, 0x02, {0}, 0xFF, 0x16};


CMD_OPERATE CmdOp[12] = {ReadVersion,
  ParamterDownLoad,
  ReadParamter,
  SampleTData,
  RtuResponse,
  CmdReserve,
  ReadSnNum,
  WriteSnNum,
  CurrentTimeCheck,
  ReadCurTime,
  WarningSet,
  SampleSendSet};


