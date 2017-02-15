#include  <app_wdt.h>

//������Ź���ʱ������
SOFT_WATCH_DOG_TIMER SoftWatchDogTimerList[MAX_SWDT_ID] = {0};

u8 StopWDTFedMake = 0;


/****************************************************************************
* ��    �ƣ�void SoftWdtISR(void)
* ��    �ܣ���ʱ��ѯÿһ��������Ź�ι��״��
* ��ڲ�������
* ���ڲ�������
* ��    ע����
****************************************************************************/
void SoftWdtISR(void)
{
    SOFT_WATCH_DOG_TIMER *SoftWatchDogTimerPtr = SoftWatchDogTimerList;
    u8 i = 0;
   
    if(StopWDTFedMake == TRUE)//ֹͣӲι��
    {        
            return;
    }
    for(i=0; i<MAX_SWDT_ID; i++)
    {
        //�Թ���Ϳ��еĿ��Ź���ʱ�������м���
        if(SoftWatchDogTimerPtr->watchDogState == SWDT_STAT_RUN)        //���Ź�����̬
        {
            //������Ź���ιʳ����װ���Ź���������
            if(SoftWatchDogTimerPtr->watchDogCount != SoftWatchDogTimerPtr->watchDogCountBack)   //ι���������μ��������ϴμ������Ƚϣ�����->��ι�������->��δι����
            {     
               SoftWatchDogTimerPtr->watchDogCountBack = SoftWatchDogTimerPtr->watchDogCount;    //����ֵ������һ��ֵ
               SoftWatchDogTimerPtr->watchDogTime = SoftWatchDogTimerPtr->watchDogTimeOut;       //��������ʱ����ֵ
            }
            else if(--SoftWatchDogTimerPtr->watchDogTime == 0)//û��ι�������Ź���ʱ����һ����
            { 
               //������һ�������ŵĿ��Ź���ʱֹͣιӲ�����Ź�
               StopWDTFedMake = TRUE;
               return;
            }
        }
        SoftWatchDogTimerPtr++;
    }       
    //����Ӳ��ι��
    IWDT_Feed();
}

/****************************************************************************
* ��    �ƣ�void HardIWDTInit()
* ��    �ܣ���ʼ��IWDT���Ź�������
* ��ڲ�����1.Ԥ��Ƶ 2.��װ����ֵ
* ���ڲ�������
* ��    ע��Ԥ��Ƶֵ��IWDG_Prescaler_4(4��8��16��32��64��128��256)
*           ����ֵ��  0 �C 0x0FFF
****************************************************************************/
void HardIWDTInit(void)
{
  WDTCTL = WDTPW + WDTSSEL;     // ���Ź���λʱ��1S
}

/****************************************************************************
* ��    �ƣ�void IWDT_Feed(void)
* ��    �ܣ�Ӳ��ι��(����װ��IDWTֵ)
* ��ڲ�������
* ���ڲ�������
* ��    ע����
****************************************************************************/
void IWDT_Feed(void)
{
  WDTCTL = WDTPW + WDTSSEL + WDTCNTCL;               // �忴�Ź�����
}

/****************************************************************************
* ��    �ƣ�void SoftWDTInit(void)
* ��    �ܣ�Ӳ�����Ź���ʼ��
* ��ڲ�������
* ���ڲ�������
* ��    ע����
****************************************************************************/
void HardWDTInit(void)
{
    memset(SoftWatchDogTimerList, 0, sizeof(SOFT_WATCH_DOG_TIMER)*MAX_SWDT_ID);
    StopWDTFedMake = 0;
    //��ʼ��Ӳ�����Ź�
    HardIWDTInit();
}

/****************************************************************************
* ��    �ƣ�u8 SoftWdtInit(SWDT_ID SwdtId, uint16 TimerTop)
* ��    �ܣ���ʼ��������Ź�
* ��ڲ�����1.���Ź������
*           2.��ʱʱ�䵥λ��Ms(����)
* ���ڲ�����1.��ȷ 1
*           2.���� 0
* ��    ע����
****************************************************************************/
u8 SoftWdtInit(SWDT_ID SwdtId, u32 TimerTop)
{
    SOFT_WATCH_DOG_TIMER *SoftWatchDogTimerPtr = SoftWatchDogTimerList;
    u32 osTick = 0;
    /* ���Ź������ų�����Χ���ش��� */
    if(SwdtId >= MAX_SWDT_ID){
            return 0;
    }
    /* ������Ź�״̬�Ƿ��ڿ��� */
    if(SoftWatchDogTimerPtr->watchDogState == SWDT_STAT_IDLE){
       SoftWatchDogTimerPtr += SwdtId; //ָ��SwdtId��ŵĽṹ�����
       osTick = MsToOSTicks(TimerTop); //��mSʱ�任���ϵͳʱ�ӽ���
       SoftWatchDogTimerPtr->watchDogTimeOut = osTick;
       SoftWatchDogTimerPtr->watchDogTime = osTick;
       SoftWatchDogTimerPtr->watchDogState = SWDT_STAT_RUN;

       return 1;
    }else{
             return 0;
    }
}

/****************************************************************************
* ��    �ƣ�u16 MsToOSTicks(u16 CommanTime)
* ��    �ܣ�����ת��Ϊϵͳʱ�ӽ���
* ��ڲ�������Ҫת���ĺ���
* ���ڲ�����ת�����ϵͳʱ�ӽ�����
* ��    ע����
****************************************************************************/
u32 MsToOSTicks(u32 CommanTime)
{
  return (OS_TICKS_PER_SEC / 1000.0) * CommanTime;
}


/****************************************************************************
* ��    �ƣ�void SoftWdtFed(SWDT_ID SwdtId)
* ��    �ܣ�������Ź�ιʳ
* ��ڲ��������Ź�����ID
* ���ڲ�������
* ��    ע������ID�ҵ���Ӧ��������Ź���Ȼ��ִ��SoftWatchDogTimerPtr->watchDogCount++;
*           ����֤SoftWatchDogTimerPtr->watchDogCount ������
*           SoftWatchDogTimerPtr->watchDogCountBack
****************************************************************************/
void SoftWdtFed(SWDT_ID SwdtId)
{
      #if OS_CRITICAL_METHOD == 3u                     /* Allocate storage for CPU status register           */
      OS_CPU_SR  cpu_sr = 0u;
      #endif
      
      SOFT_WATCH_DOG_TIMER *SoftWatchDogTimerPtr = SoftWatchDogTimerList;
      if(SwdtId >= MAX_SWDT_ID){
              return;
      }
      SoftWatchDogTimerPtr += SwdtId;
      OS_ENTER_CRITICAL();
      SoftWatchDogTimerPtr->watchDogCount++;
      //��֤������ֵʹ�ղ����
      if(SoftWatchDogTimerPtr->watchDogCount ==
            SoftWatchDogTimerPtr->watchDogCountBack)
      {
           SoftWatchDogTimerPtr->watchDogCount++;
      }
        OS_EXIT_CRITICAL();
}