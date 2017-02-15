#include "hal_ds18b20.h"
#include <stdio.h>


/****************************************************************************
* Ãû    ³Æ£ºunsigned char DS18B20_Init(void)
* ¹¦    ÄÜ£º³õÊ¼»¯
* Èë¿Ú²ÎÊý£ºÎÞ
* ³ö¿Ú²ÎÊý£º·µ»Øresult
* ±¸    ×¢£ºÎÞ
****************************************************************************/
unsigned char DS18B20_Init(void)
{
  static unsigned char result;
  //DQ_IN;//
  //DQ_H;//
  //_NOP();//
  DQ_OUT;
  DQ_L;
  Delay_Us(500);   //æ€»çº¿æ‹‰ä½Ž480us~960usï¼Œå³ä¸»æœºå‘é€å¤ä½è„‰å†?
  DQ_IN;           //æ€»çº¿è®¾ä¸ºè¾“å…¥çŠ¶æ€ï¼Œç­‰å¾…18b20åº”ç­”
  Delay_Us(60);   
  result=DQ_DATE;  //è¯»å–18b20åº”ç­”ä¿¡å·
  Delay_Us(500);    //ç­‰å¾…480usä»¥ä¸Šï¼Œä¿è¯æ€»çº¿é‡Šæ”¾
  //printf("ds18b20 Signal %d\n",result);
  return(result);  //è¿”å›ž0åˆ™åˆå§‹åŒ–æˆåŠŸ 
}

/****************************************************************************
* Ãû    ³Æ£ºunsigned char DS18B20_ReadBit(void)
* ¹¦    ÄÜ£º¶ÁÎ»²Ù×÷
* Èë¿Ú²ÎÊý£ºÎÞ
* ³ö¿Ú²ÎÊý£º¶ÁÈëÒ»¸öbyte
* ±¸    ×¢£ºÎÞ
****************************************************************************/
unsigned char DS18B20_ReadBit(void)
{
  static unsigned char result;
  DQ_OUT;     //è®¾ç½®æ€»çº¿ä¸ºè¾“å‡ºçŠ¶æ€?
  DQ_L;       //æ€»çº¿æ‹‰ä½Žï¼Œäº§ç”Ÿè¯»èµ·å§‹ä¿¡å·
  _NOP();     //ä¿è¯ä½Žç”µå¹³åœ¨1usä»¥ä¸Š
  //15uså†…è¯»å–æ€»çº¿ä¸Šæ•°æ?
  DQ_IN;
  result=DQ_DATE;   //æŽ¥æ”¶æ•°æ®
  Delay_Us(60);      //å»¶æ—¶60usï¼Œä¿è¯æ€»çº¿é‡Šæ”¾
  
  return(result);   //è¿”å›žæ•°æ®
}
/****************************************************************************
* Ãû    ³Æ£ºvoid DS18B20_WriteBit(unsigned char Data)
* ¹¦    ÄÜ£ºÐ´Î»²Ù×÷
* Èë¿Ú²ÎÊý£ºÐ´ÈëÒ»¸öbyte
* ³ö¿Ú²ÎÊý£ºÎÞ
* ±¸    ×¢£ºÎÞ
****************************************************************************/
void DS18B20_WriteBit(unsigned char Data)
{
  DQ_OUT;               //Êä³ö
  DQ_L;                 //À­µÍ
  Delay_Us(15);         //15usÑÓÊ±
  
  if(Data) DQ_H; 
  else     DQ_L; 
  Delay_Us(100);        //ÑÓÊ±60 ~ 120 us
  
  DQ_IN;                //ÊäÈë
}
/****************************************************************************
* Ãû    ³Æ£ºunsigned char DS18B20_ReadByte(void)
* ¹¦    ÄÜ£º¶Á×Ö½Ú²Ù×÷
* Èë¿Ú²ÎÊý£ºÎÞ
* ³ö¿Ú²ÎÊý£º·µ»ØÒ»¸ö×Ö½ÚÊý¾Ý
* ±¸    ×¢£ºÎÞ
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
* Ãû    ³Æ£ºvoid DS18B20_WriteByte(unsigned char Data)
* ¹¦    ÄÜ£ºÐ´×Ö½Ú²Ù×÷
* Èë¿Ú²ÎÊý£ºÒ»¸ö×Ö½ÚÊý¾Ý
* ³ö¿Ú²ÎÊý£ºÎÞ
* ±¸    ×¢£ºÎÞ
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
* Ãû    ³Æ£ºvoid DS18B20_Convert(void)
* ¹¦    ÄÜ£ºÎÂ¶È×ª»»
* Èë¿Ú²ÎÊý£ºÎÞ
* ³ö¿Ú²ÎÊý£ºÎÞ
* ±¸    ×¢£ºÎÞ
****************************************************************************/
void DS18B20_Convert(void)
{
  //printf("Temperature : %ld\n",Temperature);
  DS18B20_Init(); 
  DS18B20_WriteByte(SkipRom);
  DS18B20_WriteByte(ConvertTemperature);
}

/****************************************************************************
* Ãû    ³Æ£ºunsigned short int DS18B20_ReadTemp(void)
* ¹¦    ÄÜ£º¶ÁÈ¡×ª»»ºóµÄ¼Ä´æÆ÷ÎÂ¶ÈÖµ
* Èë¿Ú²ÎÊý£ºÎÞ
* ³ö¿Ú²ÎÊý£º·µ»ØÊµ¼ÊÎÂ¶ÈÖµ
* ±¸    ×¢£ºTempValue.TempFlag = -1 Îª¸ºÎÂ¶È
*           TempValue.TempFlag = 1  ÎªÕýÎÂ¶È
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

  tempL=DS18B20_ReadByte();   //¶ÁÈ¡µÍ¼Ä´æÆ÷
  //printf("temPl:%d\n",tempL);
  tempH=DS18B20_ReadByte();   //¶ÁÈ¡¸ß¼Ä´æÆ÷
  //printf("temPH:%d\n",tempH);
  
  Temperature  = tempH; 
  Temperature <<= 8; 
  Temperature |= tempL; 
  
  /* ÎÂ¶È×ª»»³öÏÖ85´íÎó·µ»ØÒ»¸ö-1 */
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
     TempValue.TempValue = -((((~Temperature) + 1)&0x7ff)*0.0625); //¸ºÎÂ¶ÈÖµ
     TempValue.Err       = TEMPERATURE_OK;
     OS_EXIT_CRITICAL();//
  }
  else		//positive
  {
     OS_ENTER_CRITICAL();//
     TempValue.TempFlag  = POSITIVE_FLAG;
     TempValue.TempValue = 0.0625*Temperature;          //ÕýÎÂ¶ÈÖµ
     TempValue.Err       = TEMPERATURE_OK;
     OS_EXIT_CRITICAL();//
  }
  return TempValue;
}


/****************************************************************************
* Ãû    ³Æ£ºTEMP_STRUCT TemperatureSample(void)
* ¹¦    ÄÜ£º¶ÁÈ¡×ª»»ºóµÄ¼Ä´æÆ÷ÎÂ¶ÈÖµ
* Èë¿Ú²ÎÊý£ºÎÞ
* ³ö¿Ú²ÎÊý£º·µ»ØÊµ¼ÊÎÂ¶ÈÖµ
* ±¸    ×¢£ºTempValue.TempFlag = -1 Îª¸ºÎÂ¶È
*           TempValue.TempFlag = 1  ÎªÕýÎÂ¶È
****************************************************************************/
TEMP_STRUCT TemperatureSample(void)
{   
    TEMP_STRUCT T_Value = {0, 0.0};
    TEMP_POWER_ON;                //Èý¼«¹Ü¿ª¹Ø£º¿ª
    //Delay_Ms(15);
    DS18B20_Convert();
    //Delay_Ms(1000);               //ÔÚÃ»ÓÐ¿ª¹ØÁ¿µÄÊ±ºòÃ»ÓÐÑÓÊ±£¬¿ÉÒÔÕý³£²É¼¯¡£12Î»¾«¶È×ª»»ÑÓÊ±ÕâÀï×îÉÙ750MS
    OSTimeDlyHMSM(0,0,1,0);
    T_Value = DS18B20_ReadTemp();
    TEMP_POWER_OFF;               //Èý¼«¹Ü¿ª¹Ø£º¹Ø
    return T_Value;
}
