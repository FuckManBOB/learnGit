#include "hal_ds18b20.h"
#include <stdio.h>


/****************************************************************************
* ��    �ƣ�unsigned char DS18B20_Init(void)
* ��    �ܣ���ʼ��
* ��ڲ�������
* ���ڲ���������result
* ��    ע����
****************************************************************************/
unsigned char DS18B20_Init(void)
{
  static unsigned char result;
  //DQ_IN;//
  //DQ_H;//
  //_NOP();//
  DQ_OUT;
  DQ_L;
  Delay_Us(500);   //总线拉低480us~960us，即主机发送复位脉�?
  DQ_IN;           //总线设为输入状态，等待18b20应答
  Delay_Us(60);   
  result=DQ_DATE;  //读取18b20应答信号
  Delay_Us(500);    //等待480us以上，保证总线释放
  //printf("ds18b20 Signal %d\n",result);
  return(result);  //返回0则初始化成功 
}

/****************************************************************************
* ��    �ƣ�unsigned char DS18B20_ReadBit(void)
* ��    �ܣ���λ����
* ��ڲ�������
* ���ڲ���������һ��byte
* ��    ע����
****************************************************************************/
unsigned char DS18B20_ReadBit(void)
{
  static unsigned char result;
  DQ_OUT;     //设置总线为输出状�?
  DQ_L;       //总线拉低，产生读起始信号
  _NOP();     //保证低电平在1us以上
  //15us内读取总线上数�?
  DQ_IN;
  result=DQ_DATE;   //接收数据
  Delay_Us(60);      //延时60us，保证总线释放
  
  return(result);   //返回数据
}
/****************************************************************************
* ��    �ƣ�void DS18B20_WriteBit(unsigned char Data)
* ��    �ܣ�дλ����
* ��ڲ�����д��һ��byte
* ���ڲ�������
* ��    ע����
****************************************************************************/
void DS18B20_WriteBit(unsigned char Data)
{
  DQ_OUT;               //���
  DQ_L;                 //����
  Delay_Us(15);         //15us��ʱ
  
  if(Data) DQ_H; 
  else     DQ_L; 
  Delay_Us(100);        //��ʱ60 ~ 120 us
  
  DQ_IN;                //����
}
/****************************************************************************
* ��    �ƣ�unsigned char DS18B20_ReadByte(void)
* ��    �ܣ����ֽڲ���
* ��ڲ�������
* ���ڲ���������һ���ֽ�����
* ��    ע����
****************************************************************************/
unsigned char DS18B20_ReadByte(void)
{
  static unsigned char i,result=0;
  for(i=0;i<8;i++)
  {
    result>>=1;
    if(DS18B20_ReadBit())  
	result|=0x80;
    else
        result&=0x7f;
  }
  return(result); 
}
/****************************************************************************
* ��    �ƣ�void DS18B20_WriteByte(unsigned char Data)
* ��    �ܣ�д�ֽڲ���
* ��ڲ�����һ���ֽ�����
* ���ڲ�������
* ��    ע����
****************************************************************************/
void DS18B20_WriteByte(unsigned char Data)
{
  static unsigned char i;
  for(i=0;i<8;i++)
  {
    DS18B20_WriteBit(Data&0x01);
    Data>>=1;
  } 
}

/****************************************************************************
* ��    �ƣ�void DS18B20_Convert(void)
* ��    �ܣ��¶�ת��
* ��ڲ�������
* ���ڲ�������
* ��    ע����
****************************************************************************/
void DS18B20_Convert(void)
{
  //printf("Temperature : %ld\n",Temperature);
  DS18B20_Init(); 
  DS18B20_WriteByte(SkipRom);
  DS18B20_WriteByte(ConvertTemperature);
}

/****************************************************************************
* ��    �ƣ�unsigned short int DS18B20_ReadTemp(void)
* ��    �ܣ���ȡת����ļĴ����¶�ֵ
* ��ڲ�������
* ���ڲ���������ʵ���¶�ֵ
* ��    ע��TempValue.TempFlag = -1 Ϊ���¶�
*           TempValue.TempFlag = 1  Ϊ���¶�
****************************************************************************/
TEMP_STRUCT DS18B20_ReadTemp(void)
{
  static int TestCnt;
  TEMP_STRUCT TempValue = {0, 0.0, 0};
  static unsigned char tempH,tempL;
  unsigned short int Temperature = 0; 
  
  DS18B20_Init(); 
  DS18B20_WriteByte(SkipRom);
  DS18B20_WriteByte(ReadScratchpad);

  tempL=DS18B20_ReadByte();   //��ȡ�ͼĴ���
  //printf("temPl:%d\n",tempL);
  tempH=DS18B20_ReadByte();   //��ȡ�߼Ĵ���
  //printf("temPH:%d\n",tempH);
  
  Temperature  = tempH; 
  Temperature <<= 8; 
  Temperature |= tempL; 
  
  /* �¶�ת������85���󷵻�һ��-1 */
  if(Temperature == 0x0550)
  {
    TestCnt++;
    TempValue.Err = TEMPERATURE_ERR;
    return TempValue;
  }
  
  if(tempH&0x80) //negative
  {
     OS_ENTER_CRITICAL();//
     TempValue.TempFlag  = NAGETIVE_FLAG;
     TempValue.TempValue = -((((~Temperature) + 1)&0x7ff)*0.0625); //���¶�ֵ
     TempValue.Err       = TEMPERATURE_OK;
     OS_EXIT_CRITICAL();//
  }
  else		//positive
  {
     OS_ENTER_CRITICAL();//
     TempValue.TempFlag  = POSITIVE_FLAG;
     TempValue.TempValue = 0.0625*Temperature;          //���¶�ֵ
     TempValue.Err       = TEMPERATURE_OK;
     OS_EXIT_CRITICAL();//
  }
  return TempValue;
}


/****************************************************************************
* ��    �ƣ�TEMP_STRUCT TemperatureSample(void)
* ��    �ܣ���ȡת����ļĴ����¶�ֵ
* ��ڲ�������
* ���ڲ���������ʵ���¶�ֵ
* ��    ע��TempValue.TempFlag = -1 Ϊ���¶�
*           TempValue.TempFlag = 1  Ϊ���¶�
****************************************************************************/
TEMP_STRUCT TemperatureSample(void)
{   
    TEMP_STRUCT T_Value = {0, 0.0};
    TEMP_POWER_ON;                //�����ܿ��أ���
    //Delay_Ms(15);
    DS18B20_Convert();
    //Delay_Ms(1000);               //��û�п�������ʱ��û����ʱ�����������ɼ���12λ����ת����ʱ��������750MS
    OSTimeDlyHMSM(0,0,1,0);
    T_Value = DS18B20_ReadTemp();
    TEMP_POWER_OFF;               //�����ܿ��أ���
    return T_Value;
}
