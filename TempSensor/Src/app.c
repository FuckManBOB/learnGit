#include "includes.h"


/*
*********************************************************************************************************
*                                          Device Basic Parameter
*********************************************************************************************************
*/
#ifdef  PROGRAM_PARAMETER_DOWNLOAD
/* ------------------------------- SN SYMBOL ------------------------------ */
#define DEV_ID_H                0x00                    //�豸�ŵ��ֽ�
#define DEV_ID_L                0x01                    //�豸�Ÿ��ֽ�
#define BATCH                   0x01                    //�豸����
#define YEAR_H                  0x07                    //�豸������ѵ��ֽ�
#define YEAR_L                  0xDE                    //�豸������ݸ��ֽ�
/* ------------------------- BASIC DOWNLOAD SYMBOL ------------------------ */
#define SP_INTERVAL             0x01                    //�ɼ����  ��Χ 1 -59min
#define SD_INTERVAL             0x0a                    //���ͼ��  ��Χ 1 -59min
#define WARN_ULIMIT             0x46                    //Ԥ������  30 -80 ��
#define WARN_LLIMIT             0x1E                    //Ԥ������  -35 - 0��
#define B_YEAR_L                0xDE                    //����ֽ�   2010 - 2105 ��
#define B_YEAR_H                0x07                    //����ֽ�
#define MONTH                   0x07                    //��         1 - 12 ��
#define DAY                     0x16                    //��         1 - 31 ��
#define HOUR                    0x10                    //ʱ         0 - 23 ʱ
#define MINITES                 0x32                    //��         0 - 59 ��
#define SECONDS                 0x08                    //��         0 - 59 ��
#define VERSION_L               0x02                    //�汾�ŵ��ֽ�  
#define VERSION_H               0x01                    //�汾�Ÿ��ֽ�
#define RESERVE_1               0xFF                    //�����ֽ�
#define RESERVE_2               0xFF                    //�����ֽ�

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
OS_EVENT *CMDMbox;              //����������������汾�š�����������SN��
OS_EVENT *WdHandleMbox;         //�¶Ȳɼ���������
OS_EVENT *ResponseMbox;         //�������䣺��Ӧ֡����
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
  /* �ر�Ӳ�����Ź� */
  WDTCTL = WDTPW + WDTHOLD;
  
/* �豸���������أ�����������SN����(�ɺ궨�����) */
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
    
    /* ����ϵͳʱ�� */
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
     /* ���Ͳɼ������� */
     OSTaskCreate(TaskTSD,
                 (void *)0, 
                 &TaskTSDStk[TASK_TSD_STK_SIZE - 1], 
                 APP_TASK_TSD_PRIO);
     
     /* ����ִ�� */
     OSTaskCreate(TaskCMDOP,
                 (void *)0, 
                 &TaskCMDOPStk[TASK_CMDOP_STK_SIZE - 1], 
                 APP_TASK_CMDOP_PRIO);               
}


/* ��ʱ�����¶����� */
/****************************************************************************
* ��    �ƣ�static void TaskTSD(void *pdata)
* ��    �ܣ���ʱ�����¶Ȳɼ�ֵ              
* ��ڲ�����void *pdata
* ���ڲ�������
* ��    ע����
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

/* ����ִ�� */
/****************************************************************************
* ��    �ƣ�static void TaskCMDOP(void *pdata)
* ��    �ܣ���λ���͸��豸�Ĺ������1.���汾�� 2.д���� 3.������ 4.�ɼ���������
*           5.��SN�� 6.дSN�� 7.ʱ��У׼ 8.��ȡʱ�� 9.������Ԥ������ 10.�ɼ�������
*           ������� 
*                       
* ��ڲ�����void *pdata
* ���ڲ�������
* ��    ע����
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
          case 8:                                              //ʱ��У׼
            CmdOp_CTC();
            break;
#endif
          default:
            break;
        }     
      }
    }
}