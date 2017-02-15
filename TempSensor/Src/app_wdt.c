#include  <app_wdt.h>

//软件看门狗定时器数组
SOFT_WATCH_DOG_TIMER SoftWatchDogTimerList[MAX_SWDT_ID] = {0};

u8 StopWDTFedMake = 0;


/****************************************************************************
* 名    称：void SoftWdtISR(void)
* 功    能：定时查询每一个软件看门狗喂狗状况
* 入口参数：无
* 出口参数：无
* 备    注：无
****************************************************************************/
void SoftWdtISR(void)
{
    SOFT_WATCH_DOG_TIMER *SoftWatchDogTimerPtr = SoftWatchDogTimerList;
    u8 i = 0;
   
    if(StopWDTFedMake == TRUE)//停止硬喂狗
    {        
            return;
    }
    for(i=0; i<MAX_SWDT_ID; i++)
    {
        //对挂起和空闲的看门狗定时器不进行检查获
        if(SoftWatchDogTimerPtr->watchDogState == SWDT_STAT_RUN)        //看门狗运行态
        {
            //软件看门狗有喂食，重装看门狗计数上限
            if(SoftWatchDogTimerPtr->watchDogCount != SoftWatchDogTimerPtr->watchDogCountBack)   //喂过狗（本次计数器与上次计数器比较：不等->已喂狗，相等->还未喂狗）
            {     
               SoftWatchDogTimerPtr->watchDogCountBack = SoftWatchDogTimerPtr->watchDogCount;    //将新值赋给上一次值
               SoftWatchDogTimerPtr->watchDogTime = SoftWatchDogTimerPtr->watchDogTimeOut;       //将计数超时赋初值
            }
            else if(--SoftWatchDogTimerPtr->watchDogTime == 0)//没有喂狗，看门狗计时器减一操作
            { 
               //其中任一个运行着的看门狗超时停止喂硬件看门狗
               StopWDTFedMake = TRUE;
               return;
            }
        }
        SoftWatchDogTimerPtr++;
    }       
    //调用硬件喂狗
    IWDT_Feed();
}

/****************************************************************************
* 名    称：void HardIWDTInit()
* 功    能：初始化IWDT看门狗并开启
* 入口参数：1.预分频 2.重装计数值
* 出口参数：无
* 备    注：预分频值：IWDG_Prescaler_4(4、8、16、32、64、128、256)
*           计数值：  0 – 0x0FFF
****************************************************************************/
void HardIWDTInit(void)
{
  WDTCTL = WDTPW + WDTSSEL;     // 看门狗复位时间1S
}

/****************************************************************************
* 名    称：void IWDT_Feed(void)
* 功    能：硬件喂狗(重新装载IDWT值)
* 入口参数：无
* 出口参数：无
* 备    注：无
****************************************************************************/
void IWDT_Feed(void)
{
  WDTCTL = WDTPW + WDTSSEL + WDTCNTCL;               // 清看门狗计数
}

/****************************************************************************
* 名    称：void SoftWDTInit(void)
* 功    能：硬件看门狗初始化
* 入口参数：无
* 出口参数：无
* 备    注：无
****************************************************************************/
void HardWDTInit(void)
{
    memset(SoftWatchDogTimerList, 0, sizeof(SOFT_WATCH_DOG_TIMER)*MAX_SWDT_ID);
    StopWDTFedMake = 0;
    //初始化硬件看门狗
    HardIWDTInit();
}

/****************************************************************************
* 名    称：u8 SoftWdtInit(SWDT_ID SwdtId, uint16 TimerTop)
* 功    能：初始化软件看门狗
* 入口参数：1.看门狗任务号
*           2.超时时间单位是Ms(毫秒)
* 出口参数：1.正确 1
*           2.错误 0
* 备    注：无
****************************************************************************/
u8 SoftWdtInit(SWDT_ID SwdtId, u32 TimerTop)
{
    SOFT_WATCH_DOG_TIMER *SoftWatchDogTimerPtr = SoftWatchDogTimerList;
    u32 osTick = 0;
    /* 看门狗任务编号超出范围返回错误 */
    if(SwdtId >= MAX_SWDT_ID){
            return 0;
    }
    /* 软件看门狗状态是否处于空闲 */
    if(SoftWatchDogTimerPtr->watchDogState == SWDT_STAT_IDLE){
       SoftWatchDogTimerPtr += SwdtId; //指向SwdtId编号的结构体变量
       osTick = MsToOSTicks(TimerTop); //将mS时间换算成系统时钟节拍
       SoftWatchDogTimerPtr->watchDogTimeOut = osTick;
       SoftWatchDogTimerPtr->watchDogTime = osTick;
       SoftWatchDogTimerPtr->watchDogState = SWDT_STAT_RUN;

       return 1;
    }else{
             return 0;
    }
}

/****************************************************************************
* 名    称：u16 MsToOSTicks(u16 CommanTime)
* 功    能：毫秒转换为系统时钟节拍
* 入口参数：需要转换的毫秒
* 出口参数：转换后的系统时钟节拍数
* 备    注：无
****************************************************************************/
u32 MsToOSTicks(u32 CommanTime)
{
  return (OS_TICKS_PER_SEC / 1000.0) * CommanTime;
}


/****************************************************************************
* 名    称：void SoftWdtFed(SWDT_ID SwdtId)
* 功    能：软件看门狗喂食
* 入口参数：看门狗任务ID
* 出口参数：无
* 备    注：根据ID找到相应的软件看门狗，然后执行SoftWatchDogTimerPtr->watchDogCount++;
*           并保证SoftWatchDogTimerPtr->watchDogCount 不等于
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
      //保证这两个值使终不相等
      if(SoftWatchDogTimerPtr->watchDogCount ==
            SoftWatchDogTimerPtr->watchDogCountBack)
      {
           SoftWatchDogTimerPtr->watchDogCount++;
      }
        OS_EXIT_CRITICAL();
}