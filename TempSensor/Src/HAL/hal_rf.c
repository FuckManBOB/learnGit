#include "hal_rf.h"
/* Private typedef -----------------------------------------------------------*/


/* Private define ------------------------------------------------------------*/


/* Private variables ---------------------------------------------------------*/
char *pDataByte;
u32  RfDataLength;

/* Private functions ---------------------------------------------------------*/
int RSSI_Draw(char *Buffer);



/****************************************************************************
* ��    �ƣ�void RF_Send_Data(char *strptr, u8 len)
* ��    �ܣ����Ͷ������
* ��ڲ���1���ַ����׵�ַ
* ��ڲ���2���ַ�������
* ���ڲ�������
* ��    ע����
****************************************************************************/
void RF_Send_Data(char *strptr, u32 len)
{   
    pDataByte    = strptr;
    RfDataLength = len; 
    RF_UTXEN;                                      //ʹ�ܷ����ж�
}



/****************************************************************************
* ��    �ƣ�int RF_CMD(char *cmd, char *response, u8 timeout)
* ��    �ܣ�RF�����
* ��ڲ�����1.��������
*           2.��Ӧ����
*           3.��ʱʱ��
* ���ڲ�����1.��ȷ 1
*           2.���� 0
*           3.rssiֵ-87~8
*           4.����100
* ��    ע��������QMA����ʱΪ�����ַ���ֵ��
*           ������100��Ϊ���أ��ܿ�RSSI��ֵ-87~8 
*           ��ȷ�򷵻�-87 ~ 8
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
        }                          //�ڵ��Թ����г��ֳ�ʱʱҲ�ܷ���RSSI��ֵ�����ԶԷ��س���15�����ٽ�ֵ�жϡ�
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
* ��    �ƣ�int RSSI_Draw(char *Buffer)
* ��    �ܣ���ȡQMA��RSSI����
* ��ڲ�����char *Buffer QMA����ֵ
* ���ڲ���������rssi(-87 ~ 8)  
* ��    ע�����ش�����100��Ϊ��ǣ��ܿ�RSSI��ֵ-87~8 
*           ��QMA����ķ���ֵ��ȡRSSIֵ(û�л��Ե���ģʽ0)
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
* ��    �ƣ�int RSSI_Get(void)
* ��    �ܣ���ȡRSSI����
* ��ڲ�������
* ���ڲ�����int result  
* ��    ע��ÿ�ν���ATģʽ����ȡRSSI�����˳�ATģʽ��͸��ģʽ 
*           ÿ������ԼΪ6s
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

