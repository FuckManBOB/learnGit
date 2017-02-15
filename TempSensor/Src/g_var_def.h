#ifndef __G_VAR_DEF__H
#define __G_VAR_DEF__H

//#define PROGRAM_PARAMETER_DOWNLOAD                     //定义该宏则使用程序直接将参数写入FLASH，否则不使用程序直接写入FLASH(通过无线传参) 
#define SN_DOWNLOAD                     0              //宏定义开关        1-使用    0-禁止
#define BASIC_DOWNLOAD                  0              //宏定义开关        1-使用    0-禁止
#define LED_DEBUG                       0              //led系统调试指示灯

#define CPU_F           ((double)8000000)
#define Delay_Us(x)     __delay_cycles((long)(CPU_F*(double)x/1000000.0))
#define Delay_Ms(x)     __delay_cycles((long)(CPU_F*(double)x/1000.0))
#define Delay_S(x)      __delay_cycles((long)(CPU_F*(double)x/1.0))

typedef unsigned char      u8;
typedef unsigned short int u16;
typedef short int          s16;
typedef unsigned long      u32;
typedef long               s32;

typedef volatile u32 vu32;
typedef volatile u16 vu16;
typedef volatile u8 vu8;



/*
*********************************************************************************************************
*                                            功能命令开关
*********************************************************************************************************
*/
#define D_RTU_RESPONSE                0              //重传回应            1:打开功能 0:关闭功能
#define D_READ_VERSION                0              //读版本号            1:打开功能 0:关闭功能
#define D_PARAMTER_DOWNLOAD           0              //写参数              1:打开功能 0:关闭功能
#define D_READ_PARAMTER               0              //读参数              1:打开功能 0:关闭功能
#define D_SAMPLE_TDATA                0              //从站采集发送数据    1:打开功能 0:关闭功能(暂时不使用该功能，有问题)
#define D_READ_SN_NUM                 0              //读SN号              1:打开功能 0:关闭功能
#define D_WRITE_SN_NUM                0              //写SN号              1:打开功能 0:关闭功能
#define D_TIME_CHECK                  1              //时间校准            1:打开功能 0:关闭功能
#define D_READ_CUR_TIME               0              //读当前时间          1:打开功能 0:关闭功能
#define D_WARNING_SET                 0              //上下限报警设置      1:打开功能 0:关闭功能
#define D_SSI_SET                     0              //采集间隔发送间隔设置1:打开功能 0:关闭功能



/* 由于捕捉F8913信号存在模式切换导致应答不稳定影响程序，暂且假设信号满 关闭该功能,电量显示上同*/
#define RSSI_TEST                       0
#define BATTERY_TEST                    1
#define RETRANSMISSION_TIMES            3                       //采集发送重传次数
#define RETRANSMISSION_OVER_TIME        64*3                    //超时时间(64*tick = 1024毫秒)(当前tick为16ms)
/*********************************************************************************/
#define DEVICE_TYPE             0x01

#define DEBUG   1
#define	FAIL	0		        //返回值失败
#define SECCESS !FAIL	                //返回值成功

#define APP_ERR 0                       //错误
#define APP_OK  1                       //正确

#define CONFIG_BUFFER_LENGTH    100
#define RF_BUFFER_LENGTH        250
#define CMD_MODE_BUFFER_LENGTH  50

#define DECIMALS                10      //LCD显示整数，去掉小数：X*10

#define CMD_MODE        0
#define TC_MODE         1
#define CMD_MODE_ON     CurMode = CMD_MODE
#define TC_MODE_ON      CurMode = TC_MODE


/* Global typedef -----------------------------------------------------------*/
/* RF 相关全局变量 */
extern char RfBuf[RF_BUFFER_LENGTH];
extern s16  RfIndex;

/* Config 相关全局变量 */
extern char ConfigBuf[CONFIG_BUFFER_LENGTH];	//用于接收配置信息
extern s16  ConfigBufIndex; 		        //用于接收配置信息计数

extern char CmdModeBuffer[50];
extern u8   CmdModeBufferIndex; 

extern OS_EVENT *CMDMbox;              //命令邮箱包含：读版本号、读参数、读SN号
extern OS_EVENT *ResponseMbox;         //命令邮箱：回应帧邮箱

/*
*********************************************************************************************************
*                                            基本参数
*********************************************************************************************************
*/
extern volatile char  Paramter_SampleTime;                        //采集间隔
extern volatile char  Paramter_SendTime;                          //发送间隔
extern volatile char  Paramter_WarningUpperLimit;                 //报警上限
extern volatile char  Paramter_WarningLowerLimit;                 //报警下限
extern volatile u16   Paramter_Year;                              //年
extern volatile char  Paramter_Month;                             //月
extern volatile char  Paramter_Day;                               //日
extern volatile char  Paramter_Hour;                              //时
extern volatile char  Paramter_Min;                               //分
extern volatile char  Paramter_Sec;                               //秒

extern u8  Paramter_Sn[6];                                        //SN号

extern u32 CurTime;                                               //当前时间(秒)
extern u8  CurMode;                        //当前模式：0-命令模式 1-透传模式
extern u8  RfMsg;                          //射频睡眠邮箱消息：用来传递消息值
extern u8  SoftWatchDogOverTime;           //软件看门狗溢出标志
/*
*********************************************************************************************************
*                                            主站命令帧
*********************************************************************************************************
*/
#define   CMD_WORD                        0x1F             //控制命令字后5位
#define   READ_VERSION                    0x00             //读版本号
#define   PARAMTER_DOWNLOAD               0x01             //基本参数下载
#define   READ_PARAMTER                   0x02             //读参数
#define   SAMPLE_TDATA                    0x03             //从站采集数据发送
#define   RTU_RESPONSE                    0x04             //RTU的回应帧
#define   READ_SN_NUM                     0x06             //读SN号
#define   WRITE_SN_NUM                    0x07             //写SN号
#define   TIME_CHECK                      0x08             //时间校验
#define   READ_CUR_TIME                   0x0a             //读取当前时间
#define   WARNING_SET                     0x0b             //温度/压力报警设置
#define   SSI_SET                         0x0c             //采集、发送间隔设置
/*
*********************************************************************************************************
*                                            从站正常/异常应答帧
*********************************************************************************************************
*/
#define  S_PARAMTER_DOWNLOAD_OK           0x81             //基本参数下载_正常_回应
#define  S_PARAMTER_DOWNLOAD_ERR          0xC1             //基本参数下载_异常_回应

#define  S_READ_VERSION                   0x80             //读版本号_回应
#define  S_READ_PARAMTER                  0x82             //读参数_回应
#define  S_SAMPLE_TDATA                   0x83             //从站采集数据发送_回应

#define  S_READ_SN_NUM_OK                 0x86             //读SN号_正常_回应

#define  S_WRITE_SN_NUM_OK                0x87             //写设备地址_正常_回应
#define  S_WRITE_SN_NUM_ERR               0xC7             //写设备地址_异常_回应

#define  S_TIME_CHECK_OK                  0x88             //时间校验_正常_回应
#define  S_TIME_CHECK_ERR                 0xC8             //时间校验_异常_回应

#define  S_WARNING                        0x89             //超限报警

#define  CMD_WORD_TS                      0x85             //定时发送数据命令字(被动的)

#define  S_WARNING_SET_OK                 0x8B             //温度/压力报警设置_正常_回应
#define  S_WARNING_SET_ERR                0xCB             //温度/压力报警设置_异常_回应

#define  S_SSI_SET_OK                     0x8C             //采集/发送间隔设置_正常_回应
#define  S_SSI_SET_ERR                    0xCC             //采集/发送间隔设置_异常_回应

#define  S_READ_CUR_TIME                  0x8A             //读取当前时间_回应
/*
*********************************************************************************************************
*                                            命令编号
*********************************************************************************************************
*/

typedef enum{
  ReadVersion,          //0
  ParamterDownLoad,     //1
  ReadParamter,         //2
  SampleTData,          //3
  RtuResponse,          //4
  CmdReserve,           //5
  ReadSnNum,            //6
  WriteSnNum,           //7
  CurrentTimeCheck,     //8
  ReadCurTime,          //9
  WarningSet,           //10
  SampleSendSet,        //11
}CMD_OPERATE;  

/*
*********************************************************************************************************
*                                            对应表FLASH_A
*********************************************************************************************************
*/
#define  FA_SN_ADDR_START       (char  *)0x1080
#define  FA_RTU_NUM             *((u16 *)0x1080)



/*
*********************************************************************************************************
*                                            对应表FLASH_B
*********************************************************************************************************
*/
#define   FB_PARAMTER_ADDR                (char *)0x1000
#define   FB_SAMPLE_TIME               *((char  *)0x1000)
#define   FB_SEND_TIME                 *((char  *)0x1001)
#define   FB_WARNING_ULIMIT   *((unsigned char  *)0x1002)
#define   FB_WARNING_LLIMIT   *((unsigned char  *)0x1003)
#define   FB_YEAR                      *((u16   *)0x1004)
#define   FB_MONTH                     *((char  *)0x1006)
#define   FB_DAY                       *((char  *)0x1007)
#define   FB_HOUT                      *((char  *)0x1008)
#define   FB_MINITES                   *((char  *)0x1009)
#define   FB_SECONDS                   *((char  *)0x100a)
#define   FB_VERSION                   *((u16   *)0x100b)
#define   FB_DATE_ADDR                    (char *)0x1004
#define   FB_WARN_SET                     (char *)0x1002
#define   FB_SSI_SET                    FB_PARAMTER_ADDR


#define   RESERVE_WORD        0xFFFFFFFF               //保留字默认为0xFF
/*
*      所需发送的数据包格式
*/
#pragma pack(1)
typedef struct{
    u32                 CycleNumber;            //周期编号
    u8                  PackageCnt;             //包数量
    u8                  PackageNumber;          //包号
    u32                 Time;                   //采集时间
    u8                  DataLength;             //数据长度
    float               SampleData;             //采集数据内容
    float               BatteryCharge;          //电池电量
    u32                 Reserve;                //保留
}DATA_FORMAT;
#pragma pack()

/*
*      标准帧格式
*/
#pragma pack(1)
typedef struct
{
    u8            PrefixCharacters[3];    //前导符
    u8            StartSymbol_1;          //起始符
    u8            SlaveAddr[6];           //从站地址
    u8            StartSymbol_2;          //起始符
    u8            CmdWord;                //控制字
    u8            DataLength;             //数据长度
    DATA_FORMAT   Data;                   //标准帧总长度12+26+3
    u8            CheckSum;               //校验码
    u8            EndSymbol;              //结束符
}STD_FRAM;
#pragma pack()

/*
*      版本号帧
*/
#pragma pack(1)
typedef struct
{
    u8            PrefixCharacters[3];    //前导符
    u8            StartSymbol_1;          //起始符
    u8            SlaveAddr[6];           //从站地址
    u8            StartSymbol_2;          //起始符
    u8            CmdWord;                //控制字
    u8            DataLength;             //数据长度
    u16           Version;                //版本号
    u8            CheckSum;               //校验码
    u8            EndSymbol;              //结束符
}VSN_FRAM;
#pragma pack()

/*
*      参数下载帧回应
*/
#pragma pack(1)
typedef struct
{
    u8            PrefixCharacters[3];    //前导符
    u8            StartSymbol_1;          //起始符
    u8            SlaveAddr[6];           //从站地址
    u8            StartSymbol_2;          //起始符
    u8            CmdWord;                //控制字
    u8            DataLength;             //数据长度
    u8            Paramter[15];           //参数信息
    u8            CheckSum;               //校验码
    u8            EndSymbol;              //结束符
}PARAMTER_FRAM;
#pragma pack()

/*
*      SN号帧回应
*/
#pragma pack(1)
typedef struct
{
    u8            PrefixCharacters[3];    //前导符
    u8            StartSymbol_1;          //起始符
    u8            SlaveAddr[6];           //从站地址
    u8            StartSymbol_2;          //起始符
    u8            CmdWord;                //控制字
    u8            DataLength;             //数据长度
    u8            SnNumber[6];            //SN号信息
    u8            CheckSum;               //校验码
    u8            EndSymbol;              //结束符
}SN_FRAM;
#pragma pack()

/*
*      时间帧回应
*/
#pragma pack(1)
typedef struct
{
    u8            PrefixCharacters[3];    //前导符
    u8            StartSymbol_1;          //起始符
    u8            SlaveAddr[6];           //从站地址
    u8            StartSymbol_2;          //起始符
    u8            CmdWord;                //控制字
    u8            DataLength;             //数据长度
    u8            CurTime[7];             //时间信息:年月日时分秒
    u8            CheckSum;               //校验码
    u8            EndSymbol;              //结束符
}TIME_FRAM;
#pragma pack()

/*
*      预警帧回应
*/
#pragma pack(1)
typedef struct
{
    u8            PrefixCharacters[3];    //前导符
    u8            StartSymbol_1;          //起始符
    u8            SlaveAddr[6];           //从站地址
    u8            StartSymbol_2;          //起始符
    u8            CmdWord;                //控制字
    u8            DataLength;             //数据长度
    u8            WarnSet[2];             //预警信息:上下限
    u8            CheckSum;               //校验码
    u8            EndSymbol;              //结束符
}WARN_FRAM;
#pragma pack()

/*
*      发送、采集间隔设置帧回应
*/
#pragma pack(1)
typedef struct
{
    u8            PrefixCharacters[3];    //前导符
    u8            StartSymbol_1;          //起始符
    u8            SlaveAddr[6];           //从站地址
    u8            StartSymbol_2;          //起始符
    u8            CmdWord;                //控制字
    u8            DataLength;             //数据长度
    u8            SSISet[2];              //采集、发送间隔信息
    u8            CheckSum;               //校验码
    u8            EndSymbol;              //结束符
}SSI_FRAM;
#pragma pack()


/*
*      SN号结构体
*/
#pragma pack(1)
typedef struct
{
    u16           SN_DevId;          //设备号
    u8            SN_Batch;          //批次
    u16           SN_Year;           //年
    u8            SN_DevType;        //设备类型
}SN_STRUCT;
#pragma pack()


extern STD_FRAM        StdFram;
extern DATA_FORMAT     SubFram;
extern VSN_FRAM        VsnFram;
extern PARAMTER_FRAM   ParamterFram;
extern SN_FRAM         SnFram;
extern TIME_FRAM       TimeFram;
extern WARN_FRAM       WarnFram;
extern SSI_FRAM        SsiFram;
extern SN_STRUCT       Sn_Struct; 
extern CMD_OPERATE CmdOp[12];
extern volatile u32 LocalTime;
#endif