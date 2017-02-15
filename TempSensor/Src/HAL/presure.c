#include "msp430f1611.h"
#include <stdio.h>
#include "main.h"
#include "presure.h"

unsigned char temp1,temp2,temp3,temp4;

/********************************************************************
�������ƣ����Ĵ���
��    �ܣ�
��    ����      
����ֵ  ��
*********************************************************************/
unsigned char ADS1248_ReadReg(unsigned char RegAddr)  
{    
     unsigned char Readregs;
     Ads1248SPI(ADC_CMD_RREG + (RegAddr&0X0f)); 
     Ads1248SPI(0x00);
     Readregs=Ads1248SPI(ADC_CMD_NOP);
     return Readregs;
}  
 
/********************************************************************
�������ƣ�д�Ĵ���
��    �ܣ�
��    ����      
����ֵ  ��
*********************************************************************/ 
void ADS1248_WriteReg(unsigned char RegAddr,unsigned char Buffer)  
{       
    Ads1248SPI(ADC_CMD_WREG + (RegAddr&0x0f));
    Ads1248SPI(0x00);
    Ads1248SPI(Buffer); 
}  

unsigned char ADS1248_WaitBusy(void)  
{  
    unsigned int i=0;  
    ADC_SPI_CS_CLR ; 
  
    while(ADC_SPI_OUT>0){  
        Delay_Ms(10);  
        i++; 
        if(i>200) return 1;  
    }  
    ADC_SPI_CS_SET ; 
    return 0;  
}  


/********************************************************************
�������ƣ�У׼
��    �ܣ�
��    ����      
����ֵ  ��
*********************************************************************/ 
unsigned char ADS1248_Calibrate(unsigned char Gain)  
{  
    unsigned char R=0;
    ADS1248_WriteReg(ADC_REG_SYS0,Gain);     
    ADS1248_WriteReg(ADC_REG_MUX1,0x00);        //�ָ�����ģʽ   
    Ads1248SPI(ADC_CMD_SELFOCAL);               //ϵͳƫ����У׼   
    R|=ADS1248_WaitBusy();                      //�ȴ�У׼���   
   
    ADS1248_WriteReg(ADC_REG_MUX1,0x01);       //����AINP+AINN=(AVDD+AVSS)/2    
    Ads1248SPI(ADC_CMD_SYSOCAL);               //ϵͳƫ����У׼   
    R|=ADS1248_WaitBusy();                     //�ȴ�У׼���   
   
    ADS1248_WriteReg(ADC_REG_MUX1,0x02);       //����AINP=VREF+,AINN=VREF-; for gain calibration   
    Ads1248SPI(ADC_CMD_SYSGCAL);               //ϵͳ����У׼   
    R|=ADS1248_WaitBusy();                        
  
    return R;   
}  

unsigned char ADS1248_Init(unsigned char Gain)  
{  
    ADC_START_SET;
    ADC_SPI_CS_CLR;   
    ADC_RESET_CLR; 
    Delay_Ms(100);
    ADC_RESET_SET;
    Delay_Ms(100);    
    Ads1248SPI(ADC_CMD_RESET);
    Delay_Ms(100);               
    ADS1248_WriteReg(ADC_REG_IDAC0,0x08);
    if(ADS1248_Calibrate(Gain)>0) 
      return 1;
    else 
      return 0;
}  

/*--------------------------------------------------------- 
 ����ת������ 
---------------------------------------------------------*/  
void ADS1248_Config(unsigned char CovGain,unsigned char CovRate)  
{  
    ADS1248_WriteReg(ADC_REG_MUX0,0x01);                   
    temp1=ADS1248_ReadReg(ADC_REG_MUX0);
    
    ADS1248_WriteReg(ADC_REG_VBIAS,0x00);                  
    temp2=ADS1248_ReadReg(ADC_REG_VBIAS);
    
    ADS1248_WriteReg(ADC_REG_MUX1,0x08);   
    Delay_Ms(100);  
    temp3=ADS1248_ReadReg(ADC_REG_MUX1);
    
    ADS1248_WriteReg(ADC_REG_IDAC1,0xff);     
    ADS1248_WriteReg(ADC_REG_SYS0,CovGain|CovRate);
    Delay_Ms(100); 
    temp2=ADS1248_ReadReg(ADC_REG_SYS0);  
}  
  
/*--------------------------------------------------------- 
 ����ת�� 
---------------------------------------------------------*/  
void ADS1248_Start(unsigned char CovMode)  
{  
    ADC_START_SET;                           //����ADC����   
    if(CovMode==ADC_MODE_SINGLECOV) 
      ADC_START_CLR;                         //������������   
}                                  
  
/*--------------------------------------------------------- 
ֹͣת��������͹���ģʽ 
---------------------------------------------------------*/  
void ADS1248_Stop(void)  
{  
  ADC_START_CLR;                            //ֹͣת��   
}  

/*--------------------------------------------------------- 
 ���ݶ�ȡ 
---------------------------------------------------------*/  
unsigned long int ADS1248_Read(void)  
{   
    unsigned long int DataS=0;
    Ads1248SPI(ADC_CMD_RDATA);
    DataS=Ads1248SPI(ADC_CMD_NOP);
    DataS=(DataS<<8)|Ads1248SPI(ADC_CMD_NOP); 
    DataS=(DataS<<8)|Ads1248SPI(ADC_CMD_NOP);
    Ads1248SPI(ADC_CMD_NOP);
    return DataS;  
}  

/***********************************************************************************
* �������ƣ�Compositor;
*
* ������������������ȡ�м�ֵ;
*              
* ���������unsigned long int *x,char number;
*
* �������ݣ�x[number]��
*
* ע�⣺    none;
************************************************************************************/
unsigned long int Compositor(unsigned long int *x,unsigned char AD_number)		
{
 unsigned char i, j;
 unsigned long int value=0;

 for(i=0; i<AD_number-2; i++)
  {
  for(j=i+1; j<AD_number-1; j++)
    {
     if(x[i]>x[j])
       {
         value=x[i];
         x[i]=x[j];
         x[j]=value;
       }
     }
   }
 return x[AD_number/2];
 }

unsigned long int GatherData(unsigned long int *x,unsigned char AD_number)	   
{
  unsigned char i;
  unsigned long int AD_result=0;				   
  for(i=0; i<AD_number; i++)
    {
      x[i]=ADS1248_Read();
      Delay_Ms(2);
    }
  AD_result=Compositor(x,AD_number);
  return  AD_result;
}



