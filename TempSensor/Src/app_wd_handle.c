#include "app_wd_handle.h"

#define SAMPLE_FAIL_TIMES         3             //�ɼ�ʧ�ܴ��� 

extern _calendar_obj calendar;
extern OS_EVENT *WdHandleMbox;         //�¶Ȳɼ���������

void LcdDisplay(TEMP_STRUCT Temp_Value);
TEMP_STRUCT SampleTemperature(u8 sampleFailCnt);



/****************************************************************************
* ��    �ƣ�void WdHandle(u32 CycleNum, u32 SampleTime)
* ��    �ܣ��¶Ȳɼ��������ɼ�����ʾ����������
* ��ڲ�����1.���ڱ�� 2.�ɼ�ʱ��
* ���ڲ�������
* ��    ע��
****************************************************************************/
void WdHandle(u32 CycleNum)
{
      TEMP_STRUCT  Temp_Value;                       //�¶Ƚṹ�����
      float        BC_Data;                          //��ص�������
      
      /*1.�ɼ��¶�*/
      Temp_Value = SampleTemperature(SAMPLE_FAIL_TIMES);
      
      /*2.��ȡ��ص���*/
      BC_Data = Battery_Value_Get();
      
      /*3.Һ����ʾ*/
      LcdDisplay(Temp_Value);
      
      /*4.��������*/
      F8913D_N_SLEEP_RQ;                             //��Ƶ�豸��������׼����������
      OSTimeDlyHMSM(0,0,0,500);
      FramHandle(CurTime, Temp_Value.TempValue, BC_Data, CMD_WORD_TS, RESERVE_WORD);         //�����¶Ȱ�
      RF_Send_Data((char *)&StdFram, sizeof(STD_FRAM));                                      //��������
      OSTimeDlyHMSM(0,0,2,0);                      //�ȴ���������˯(ԭ�����жϷ��ͷ�ʽ)
      F8913D_SLEEP_RQ;                             //���ݷ����꣬�豸��������
}

/****************************************************************************
* ��    �ƣ�TEMP_STRUCT SampleTemperature(u8 sampleFailCnt)
* ��    �ܣ��ɼ��¶�
* ��ڲ�������������ɼ�ʧ�ܴ���
* ���ڲ������¶Ƚṹ��
* ��    ע�����޶�ʧ�ܴ�������һ�βɼ��ɹ��򷵻زɼ����
****************************************************************************/
TEMP_STRUCT SampleTemperature(u8 sampleFailCnt)
{
      u8  S_SampleErrCnt;                           //�ɼ��������
      TEMP_STRUCT Temp_Value = {0, 0.0, 0};         //�¶�ֵ�����¶Ⱥ�������־
      
      for(S_SampleErrCnt = sampleFailCnt; S_SampleErrCnt > 0; S_SampleErrCnt--)
      {
        Temp_Value  = TemperatureSample();          //��ȡ�¶�
        if(Temp_Value.Err != TEMPERATURE_ERR)       //�ɼ�δ�������˳��ɼ�
          break;
      }
      return Temp_Value;
}

/****************************************************************************
* ��    �ƣ�void LcdDisplay(char tempFlag)
* ��    �ܣ�LCDҺ����ʾ
* ��ڲ������¶�������־
* ���ڲ�������
* ��    ע����
****************************************************************************/
void LcdDisplay(TEMP_STRUCT Temp_Value)
{
    int temp = 0;
    if( POSITIVE_FLAG == Temp_Value.TempFlag )
    {
      LcdClean();                                       //����
      temp = (int)(Temp_Value.TempValue * DECIMALS);    //���������10��(һ��С��)��ǿת��Int
      /* ��ʾ���¶� */
      LcdPosNumber( temp );
      WriteAllData(GEWEI,0x00);                         //���쳣�ĸ�λ����
    }
    else
    {
      LcdClean();                                       //����
      if( Temp_Value.TempValue < -0.0001 && Temp_Value.TempValue > -0.1 )     //���-0.0001��Һ���޷���ʾ��ȫ����ɵ����
        temp = (int)(-0.1 * DECIMALS);                  //��-0.0001 ~ -0.1�����ڵ�ֵ����Ϊ��-0.1����Һ����ʾ������ĸ��¶Ȳ��������Ϊ-00.0
      else
        temp = (int)(Temp_Value.TempValue * DECIMALS);  //���������10��(һ��С��)��ǿת��Int
      /* ��ʾ���¶� */
      LcdNegNumber( abs(temp) );                        //����ֵת��ȡ����LCD��������
      WriteAllData(GEWEI,0x00);                         //���쳣�ĸ�λ����
    }
    
    /* �������źż����ʾ */
    Symbol_Lcd_Display();
}

/****************************************************************************
* ��    �ƣ�void IsSendTime(u32 curSeconds, u8 timePoint)
* ��    �ܣ�������ĳһʱ�̼����¶��豸���вɼ�����
* ��ڲ�����1.��ǰʱ������ 2.��Ҫ�趨�Ļ���ʱ��(��ÿʱ����ĵ�15����)
* ���ڲ������ɹ���0ʧ�ܣ�1
* ��    ע����
****************************************************************************/
int IsSendTime(u32 curSeconds, u8 timePoint)
{
  static char rcdCurMin = -1;             //��¼��ǰ������
  
  if(DateTime_Get(curSeconds) != 0)
    return 1;                   //����
  
  
  if((calendar.min % timePoint) == 0)
  {
     if(rcdCurMin != calendar.min)
     {
        rcdCurMin = calendar.min;     //��¼��ǰ�������������������ַ�����û�б仯
        //ÿ��Сʱ��ʼÿ��timePoint������Wdhandle�еĲɼ�����һ���ź�
        OSMboxPost(WdHandleMbox, (void *)1);
        return 0;
     }
  }
  return 1;
}