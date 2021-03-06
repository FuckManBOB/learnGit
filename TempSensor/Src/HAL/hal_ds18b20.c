#include "hal_ds18b20.h"
#include <stdio.h>


/****************************************************************************
* 名    称：unsigned char DS18B20_Init(void)
* 功    能：初始化
* 入口参数：无
* 出口参数：返回result
* 备    注：无
****************************************************************************/
unsigned char DS18B20_Init(void)
{
  static unsigned char result;
  //DQ_IN;//
  //DQ_H;//
  //_NOP();//
  DQ_OUT;
  DQ_L;
  Delay_Us(500);   //鎬荤嚎鎷変綆480us~960us锛屽嵆涓绘満鍙戦�佸浣嶈剦鍐?
  DQ_IN;           //鎬荤嚎璁句负杈撳叆鐘舵�侊紝绛夊緟18b20搴旂瓟
  Delay_Us(60);   
  result=DQ_DATE;  //璇诲彇18b20搴旂瓟淇″彿
  Delay_Us(500);    //绛夊緟480us浠ヤ笂锛屼繚璇佹�荤嚎閲婃斁
  //printf("ds18b20 Signal %d\n",result);
  return(result);  //杩斿洖0鍒欏垵濮嬪寲鎴愬姛 
}

/****************************************************************************
* 名    称：unsigned char DS18B20_ReadBit(void)
* 功    能：读位操作
* 入口参数：无
* 出口参数：读入一个byte
* 备    注：无
****************************************************************************/
unsigned char DS18B20_ReadBit(void)
{
  static unsigned char result;
  DQ_OUT;     //璁剧疆鎬荤嚎涓鸿緭鍑虹姸鎬?
  DQ_L;       //鎬荤嚎鎷変綆锛屼骇鐢熻璧峰淇″彿
  _NOP();     //淇濊瘉浣庣數骞冲湪1us浠ヤ笂
  //15us鍐呰鍙栨�荤嚎涓婃暟鎹?
  DQ_IN;
  result=DQ_DATE;   //鎺ユ敹鏁版嵁
  Delay_Us(60);      //寤舵椂60us锛屼繚璇佹�荤嚎閲婃斁
  
  return(result);   //杩斿洖鏁版嵁
}
/****************************************************************************
* 名    称：void DS18B20_WriteBit(unsigned char Data)
* 功    能：写位操作
* 入口参数：写入一个byte
* 出口参数：无
* 备    注：无
****************************************************************************/
void DS18B20_WriteBit(unsigned char Data)
{
  DQ_OUT;               //输出
  DQ_L;                 //拉低
  Delay_Us(15);         //15us延时
  
  if(Data) DQ_H; 
  else     DQ_L; 
  Delay_Us(100);        //延时60 ~ 120 us
  
  DQ_IN;                //输入
}
/****************************************************************************
* 名    称：unsigned char DS18B20_ReadByte(void)
* 功    能：读字节操作
* 入口参数：无
* 出口参数：返回一个字节数据
* 备    注：无
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
* 名    称：void DS18B20_WriteByte(unsigned char Data)
* 功    能：写字节操作
* 入口参数：一个字节数据
* 出口参数：无
* 备    注：无
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
* 名    称：void DS18B20_Convert(void)
* 功    能：温度转换
* 入口参数：无
* 出口参数：无
* 备    注：无
****************************************************************************/
void DS18B20_Convert(void)
{
  //printf("Temperature : %ld\n",Temperature);
  DS18B20_Init(); 
  DS18B20_WriteByte(SkipRom);
  DS18B20_WriteByte(ConvertTemperature);
}

/****************************************************************************
* 名    称：unsigned short int DS18B20_ReadTemp(void)
* 功    能：读取转换后的寄存器温度值
* 入口参数：无
* 出口参数：返回实际温度值
* 备    注：TempValue.TempFlag = -1 为负温度
*           TempValue.TempFlag = 1  为正温度
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

  tempL=DS18B20_ReadByte();   //读取低寄存器
  //printf("temPl:%d\n",tempL);
  tempH=DS18B20_ReadByte();   //读取高寄存器
  //printf("temPH:%d\n",tempH);
  
  Temperature  = tempH; 
  Temperature <<= 8; 
  Temperature |= tempL; 
  
  /* 温度转换出现85错误返回一个-1 */
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
     TempValue.TempValue = -((((~Temperature) + 1)&0x7ff)*0.0625); //负温度值
     TempValue.Err       = TEMPERATURE_OK;
     OS_EXIT_CRITICAL();//
  }
  else		//positive
  {
     OS_ENTER_CRITICAL();//
     TempValue.TempFlag  = POSITIVE_FLAG;
     TempValue.TempValue = 0.0625*Temperature;          //正温度值
     TempValue.Err       = TEMPERATURE_OK;
     OS_EXIT_CRITICAL();//
  }
  return TempValue;
}


/****************************************************************************
* 名    称：TEMP_STRUCT TemperatureSample(void)
* 功    能：读取转换后的寄存器温度值
* 入口参数：无
* 出口参数：返回实际温度值
* 备    注：TempValue.TempFlag = -1 为负温度
*           TempValue.TempFlag = 1  为正温度
****************************************************************************/
TEMP_STRUCT TemperatureSample(void)
{   
    TEMP_STRUCT T_Value = {0, 0.0};
    TEMP_POWER_ON;                //三极管开关：开
    //Delay_Ms(15);
    DS18B20_Convert();
    //Delay_Ms(1000);               //在没有开关量的时候没有延时，可以正常采集。12位精度转换延时这里最少750MS
    OSTimeDlyHMSM(0,0,1,0);
    T_Value = DS18B20_ReadTemp();
    TEMP_POWER_OFF;               //三极管开关：关
    return T_Value;
}
