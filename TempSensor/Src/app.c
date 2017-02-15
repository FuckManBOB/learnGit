#include "includes.h"


/*
*********************************************************************************************************
*                                          Device Basic Parameter
*********************************************************************************************************
*/
#ifdef  PROGRAM_PARAMETER_DOWNLOAD
/* ------------------------------- SN SYMBOL ------------------------------ */
#define DEV_ID_H                0x00                    //设备号低字节
#define DEV_ID_L                0x01                    //设备号高字节
#define BATCH                   0x01                    //设备批次
#define YEAR_H                  0x07                    //设备生产年费低字节
#define YEAR_L                  0xDE                    //设备生产年份高字节
/* ------------------------- BASIC DOWNLOAD SYMBOL ------------------------ */
#define SP_INTERVAL             0x01                    //采集间隔  范围 1 -59min
#define SD_INTERVAL             0x0a                    //发送间隔  范围 1 -59min
#define WARN_ULIMIT             0x46                    //预警上限  30 -80 ℃
#define WARN_LLIMIT             0x1E                    //预警下限  -35 - 0℃
#define B_YEAR_L                0xDE                    //年低字节   2010 - 2105 年
#define B_YEAR_H                0x07                    //年高字节
#define MONTH                   0x07                    //月         1 - 12 月
#define DAY                     0x16                    //日         1 - 31 日
#define HOUR                    0x10                    //时         0 - 23 时
#define MINITES                 0x32                    //分         0 - 59 分
#define SECONDS                 0x08                    //秒         0 - 59 分
#define VERSION_L               0x02                    //版本号低字节  
#define VERSION_H               0x01                    //版本号高字节
#define RESERVE_1               0xFF                    //保留字节
#define RESERVE_2               0xFF                    //保留字节

char SN_Parameter[]    = {DEV_ID_L, DEV_ID_H, BATCH, YEAR_L, YEAR_H, DEVICE_TYPE};

char Basic_Parameter[] = {SP_INTERVAL, SD_INTERVAL, WARN_ULIMIT, WARN_LLIMIT, B_YEAR_L,
                          B_YEAR_H, MONTH, DAY, HOUR, MINITES, SECONDS, VERSION_L, 
                          VERSION_H, RESERVE_1,RESERVE_2};
#endif
/*
*********************************************************************************************************
*                                                 DEFINES
*********************************************************************************************************
*/



/*
*********************************************************************************************************
*                                                VARIABLES
*********************************************************************************************************
*/
static OS_STK   TaskSYSStk [TASK_SYS_STK_SIZE];
static OS_STK   TaskTSDStk [TASK_TSD_STK_SIZE];
static OS_STK   TaskCMDOPStk[TASK_CMDOP_STK_SIZE];

/*
*********************************************************************************************************
*                                            MBOX DEFINITION
*********************************************************************************************************
*/
OS_EVENT *CMDMbox;              //命令邮箱包含：读版本号、读参数、读SN号
OS_EVENT *WdHandleMbox;         //温度采集处理邮箱
OS_EVENT *ResponseMbox;         //命令邮箱：回应帧邮箱
/*
*********************************************************************************************************
*                                            FUNCTION PROTOTYPES
*********************************************************************************************************
*/

static void   TaskSYSTEM           (void       *pdata);
static void   TaskTSD              (void       *pdata);
static void   TaskCMDOP            (void       *pdata);
static void   AppTaskCreate        (void             );   



 
void main(void)
{
  /* 关闭硬件开门狗 */
  WDTCTL = WDTPW + WDTHOLD;
  
/* 设备参数的下载：基本参数、SN参数(由宏定义控制) */
#ifdef PROGRAM_PARAMETER_DOWNLOAD
  #if SN_DOWNLOAD
    if(FlashWrite(FLASH_SEG_A, SN_Parameter, sizeof(SN_Parameter)) == SECCESS)
    {
      printf("OK\r\n");
    }
    else
      printf("ERR\r\n");
  #endif
#endif
    OSInit();
    OSTaskCreate(TaskSYSTEM, (void *)0, &TaskSYSStk[TASK_SYS_STK_SIZE - 1], TASK_SYS_PRIO);
    OSStart();
}


/*
*********************************************************************************************************
*                                          STARTUP TASK
*
* Description : This is an example of a startup task.  As mentioned in the book's text, you MUST
*               initialize the ticker only once multitasking has started.
*
* Arguments   : p_arg   is the argument passed to 'AppStartTask()' by 'OSTaskCreate()'.
*
* Note(s)     : 1) The first line of code is used to prevent a compiler warning because 'p_arg' is not
*                  used.  The compiler should not generate any code for this statement.
*               2) Interrupts are enabled once the task start because the I-bit of the CCR register was
*                  set to 0 by 'OSTaskCreate()'.
*********************************************************************************************************
*/

static  void  TaskSYSTEM (void *pdata)
{          
    pdata = pdata;
    
    BSP_Init();                                                         /* Initialize BSP functions                                 */
    
    /* 启动系统时钟 */
    OS_TickStart();
    
    AppTaskCreate();                                                    /* Create application tasks                                 */
      
    OSTaskDel(OS_PRIO_SELF);
}

/*
*********************************************************************************************************
*                                        CREATE APPLICATION TASKS
*
* Description: This function creates the application tasks.
*
* Arguments  : none.
*
* Note(s)    : none.
*********************************************************************************************************
*/
static  void  AppTaskCreate (void)
{      
     /* 发送采集的数据 */
     OSTaskCreate(TaskTSD,
                 (void *)0, 
                 &TaskTSDStk[TASK_TSD_STK_SIZE - 1], 
                 APP_TASK_TSD_PRIO);
     
     /* 命令执行 */
     OSTaskCreate(TaskCMDOP,
                 (void *)0, 
                 &TaskCMDOPStk[TASK_CMDOP_STK_SIZE - 1], 
                 APP_TASK_CMDOP_PRIO);               
}


/* 定时发送温度数据 */
/****************************************************************************
* 名    称：static void TaskTSD(void *pdata)
* 功    能：定时发送温度采集值              
* 入口参数：void *pdata
* 出口参数：无
* 备    注：无
****************************************************************************/
static void TaskTSD(void *pdata)
{
    u8 err;
    u8 *msg;
    static u32 CycleCnt = 1;
    
    pdata = pdata;
    
    WdHandleMbox = OSMboxCreate((void *)0);
    
    for(;;)
    {
      msg = OSMboxPend(WdHandleMbox, 0, &err);
      if( err == OS_NO_ERR )
      {
        WdHandle(CycleCnt);
        CycleCnt++;
        //OSTimeDlyHMSM(0,0,10,100);
      }
    }
}

/* 命令执行 */
/****************************************************************************
* 名    称：static void TaskCMDOP(void *pdata)
* 功    能：上位发送给设备的功能命令：1.读版本号 2.写参数 3.读参数 4.采集发送数据
*           5.读SN号 6.写SN号 7.时间校准 8.读取时间 9.上下限预警设置 10.采集、发送
*           间隔设置 
*                       
* 入口参数：void *pdata
* 出口参数：无
* 备    注：无
****************************************************************************/
static void TaskCMDOP(void *pdata)
{
    u8 err;
    u8 *msg;
    
    pdata = pdata;
    
    CMDMbox = OSMboxCreate((void *)0);
    for(;;)
    {
      msg = OSMboxPend(CMDMbox, 0, &err);
      if( err == OS_NO_ERR )
      {
        switch(*((u8 *)msg))
        {
#if D_TIME_CHECK
          case 8:                                              //时间校准
            CmdOp_CTC();
            break;
#endif
          default:
            break;
        }     
      }
    }
}