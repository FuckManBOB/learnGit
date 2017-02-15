#include "hal_rf.h"
/* Private typedef -----------------------------------------------------------*/


/* Private define ------------------------------------------------------------*/


/* Private variables ---------------------------------------------------------*/
char *pDataByte;
u32  RfDataLength;

/* Private functions ---------------------------------------------------------*/
int RSSI_Draw(char *Buffer);



/****************************************************************************
* 名    称：void RF_Send_Data(char *strptr, u8 len)
* 功    能：发送多个数据
* 入口参数1：字符串首地址
* 入口参数2：字符串长度
* 出口参数：无
* 备    注：无
****************************************************************************/
void RF_Send_Data(char *strptr, u32 len)
{   
    pDataByte    = strptr;
    RfDataLength = len; 
    RF_UTXEN;                                      //使能发送中断
}



/****************************************************************************
* 名    称：int RF_CMD(char *cmd, char *response, u8 timeout)
* 功    能：RF命令发送
* 入口参数：1.命令内容
*           2.响应内容
*           3.超时时间
* 出口参数：1.正确 1
*           2.错误 0
*           3.rssi值-87~8
*           4.错误100
* 备    注：当发送QMA命令时为后两种返回值。
*           错误用100作为返回，避开RSSI的值-87~8 
*           正确则返回-87 ~ 8
****************************************************************************/
int RF_CMD(char *cmd, char *response, u8 timeout)
{
    static int rssi;
    
    if(strcmp(cmd,QMA) == 0)
    {
        RF_Send_Data(cmd,strlen(cmd));
        while((!strstr(RfBuf, response)) && timeout)
        {
          timeout--;
          //Delay_Ms(1000);
          OSTimeDlyHMSM(0, 0, 1, 0); 
        }                          //在调试过程中出现超时时也能返回RSSI的值，所以对返回长度15进行临界值判断。
        if(((timeout == 0) && (strlen(RfBuf) > 15)) || timeout != 0)
        {
          rssi = RSSI_Draw(RfBuf);
          RfIndex = 0;
          memset(RfBuf, 0, RF_BUFFER_LENGTH);
          return rssi;
        }
        else
        {
          RfIndex = 0;
          memset(RfBuf, 0, RF_BUFFER_LENGTH);
          return 100;
        }
    }
    
    RfIndex = 0;
    memset(RfBuf, 0, RF_BUFFER_LENGTH);
    RF_Send_Data(cmd,strlen(cmd));
    while((!strstr(RfBuf, response)) && timeout)
    {
      timeout--;
      //Delay_Ms(1000);
      OSTimeDlyHMSM(0, 0, 100, 0); 
    }
    if(0 == timeout)
    {
      #if DEBUG
      printf("%s FAIL\r\n",cmd);
      #endif                
      RfIndex = 0;
      memset(RfBuf, 0, RF_BUFFER_LENGTH);
      return FAIL;           
    }
    else
    {
      RfIndex = 0;
      memset(RfBuf, 0, RF_BUFFER_LENGTH);
      #if DEBUG
      printf("%s SECCUSS\r\n",cmd);
      #endif
      return SECCESS; 
    }
}

/****************************************************************************
* 名    称：int RSSI_Draw(char *Buffer)
* 功    能：提取QMA中RSSI数据
* 入口参数：char *Buffer QMA返回值
* 出口参数：返回rssi(-87 ~ 8)  
* 备    注：返回错误用100作为标记，避开RSSI的值-87~8 
*           从QMA命令的返回值提取RSSI值(没有回显调试模式0)
****************************************************************************/
int RSSI_Draw(char *Buffer)
{
  char *ptr_start = 0; 
  char *ptr_end = 0;
  char res[5] = {0};
  unsigned char rssi_lenth = 0;
  unsigned int i = 0;
    
  ptr_start = strrchr(Buffer,',');
  ptr_end = strrchr(Buffer,'\r');
  rssi_lenth = ptr_end - ptr_start - 1;
  for(i = 0; i < rssi_lenth; i++)
    res[i] = *(++ptr_start);       //RSSI:  -87 ~ -10 
  return atoi(res);
}

/****************************************************************************
* 名    称：int RSSI_Get(void)
* 功    能：获取RSSI数据
* 入口参数：无
* 出口参数：int result  
* 备    注：每次进入AT模式，提取RSSI后再退出AT模式到透传模式 
*           每个周期约为6s
****************************************************************************/
int RF_RSSI_Get(void)
{
  static int result;
  unsigned char i;
  for(i = 0; i < 2; i++)
  {
    RF_CMD("+++",RSP,3);
    OSTimeDlyHMSM(0, 0, 0, 100);
    //Delay_Ms(100);
  }
  result = RF_CMD(QMA,QMA,2);
  RF_CMD(ESC,ESC,2);
  OSTimeDlyHMSM(0, 0, 0, 100);
  //Delay_Ms(100);
  return result;
}

