#include "hal_time.h"

volatile u32 LocalTime = 0; /* this variable is used to create a time reference incremented by 1s */

/* һ���е��·ݰ��������� */
const u8 mon_table[12]={31,28,31,30,31,30,31,31,30,31,30,31};


_calendar_obj calendar;


/****************************************************************************
* ��    �ƣ�u8 Is_Leap_Year(u16 year)
* ��    �ܣ��ж�����
* ��ڲ�������
* ���ڲ�����1�����귵��1	2��ƽ�귵��0
* ��	ע��
****************************************************************************/
u8 Is_Leap_Year(u16 year)
{			  
    if(year%4==0) //�����ܱ�4����
    { 
      if(year%100==0) 
      { 
          if(year%400==0)
               return 1;//�����00��β,��Ҫ�ܱ�400���� 	   
          else 
               return 0;   
      }
      else 
          return 1;   
    }
    else 
        return 0;	
}

/****************************************************************************************
* ��    �ƣ�u32 DateTime_Convert_Seconds(u16 syear,u8 smon,u8 sday,u8 hour,u8 min,u8 sec)
* ��    �ܣ������������ת������
* ��ڲ������ꡢ�¡��ա�ʱ���֡���
* ���ڲ�����1������ָ����ݷ��ش���0	2����ȷ�򷵻�ת���������
* ��	ע������������  1970 < syear < 2099 ����1970��1��1����Ϊʱ�ӻ���
*****************************************************************************************/
u32 DateTime_Convert_Seconds(u16 syear,u8 smon,u8 sday,u8 hour,u8 min,u8 sec)
{
    u16 t;
    u32 seccount=0;

    if(syear < 1970 || syear > 2099)
            return 0;
                   
    for(t = 1970; t < syear; t++) 		//��������ݵ��������												 
    {
      if(Is_Leap_Year(t))
            seccount+=31622400;		  	//�����������
      else 
            seccount+=31536000;       		//ƽ���������
    }

    smon-=1;

    for(t = 0; t < smon; t++)        		//��ǰ���·ݵ����������
    {
      seccount += (u32)mon_table[t]*86400;	//�·����������
      if(Is_Leap_Year(syear) && t == 1)
            seccount += 86400;			//����2�·�����һ���������          
    }
    seccount += (u32)(sday-1)*86400;		//��ǰ�����ڵ���������� 
    seccount += (u32)hour*3600;			//Сʱ������
    seccount += (u32)min*60;       		//����������
    seccount += sec;				//�������Ӽ���ȥ  
    return seccount;
}

/****************************************************************************************
* ��    �ƣ�u8 Time_Calibration(u32 CurrentTime, u32 CalibrationTime)
* ��    �ܣ�����ǰ���ڵ�������У׼�����������бȽϣ���һ����Χ�ڱ�ʾ�����������趨��ΧΪ�쳣
* ��ڲ�����1����ǰ��������         2��У׼����������
* ���ڲ�����1����������Χ���򷵻�1	2��������Χ�򷵻�0��ʾ�쳣
* ��	ע�������������1970��1��1����Ϊʱ�ӻ���
*****************************************************************************************/
u8 Time_Calibration(u32 CurrentTime, u32 CalibrationTime)
{
  u32 Temp;
  Temp = abs(CurrentTime - CalibrationTime);
  if(Temp < CALIBRATION_TIME_ERR)return NORMAL;
  else
    return ABNORMAL;
}

/****************************************************************************
* ��    �ƣ�u8 DateTime_Get(void)
* ��    �ܣ���ȡ��ǰ��ʱ��
* ��ڲ�������
* ���ڲ���������0 ����1
* ��	ע��
****************************************************************************/
u8 DateTime_Get(u32 curTime)
{
    static u16 daycnt = 0;
    u32 timecount = 0; 
    u32 temp = 0;		//������
    u16 temp1 = 0;	        //�����	  
    timecount = curTime;        //�õ��������е�ֵ(������)			 

    temp = timecount/86400;   //�õ�����(��������Ӧ��)
    if(daycnt != temp)//����һ����
    {	  
        daycnt = temp;
        temp1 = 1970;	//��1970�꿪ʼ
        while(temp >= 365)
        {				 
        if(Is_Leap_Year(temp1))//������
        {
            if(temp >= 366)
                    temp -= 366;//�����������
            else 
                    break;  
        }
        else 
            temp -= 365;	  //ƽ�� 
        temp1++;  
        }   
        calendar.w_year = temp1;//�õ����

        temp1 = 0;

        while(temp >= 28)//������һ����
        {
        if(Is_Leap_Year(calendar.w_year)&&temp1 == 1)//�����ǲ�������/2�·�
        {
            if(temp >= 29)
                    temp -= 29;//�����������
            else 
                    break; 
        }
        else 
        {
            if(temp >= mon_table[temp1])
                    temp -= mon_table[temp1];//ƽ��
            else 
                    break;
        }

        temp1++;
          
        }
        calendar.w_month = temp1+1;	//�õ��·�
        calendar.w_date  = temp+1;  	//�õ����� 
    }
    temp = timecount%86400;     	//�õ�������   	   
    calendar.hour = temp/3600;     	//Сʱ
    calendar.min  = (temp%3600)/60; 	//����	
    calendar.sec  = (temp%3600)%60; 	//����   
    return 0;
}	 

/****************************************************************************
* ��    �ƣ�u32 BasicSecond_Get(void)
* ��    �ܣ���ȡ��1970����ǰ������
* ��ڲ�������
* ���ڲ��������ص�ǰ����
* ��	ע��
****************************************************************************/	 
u32 BasicSecond_Get(void)
{
    return LocalTime;
}

/****************************************************************************
* ��    �ƣ�void SetCurrentTime(u32 CurrentTime_Seconds)
* ��    �ܣ����õ�ǰʱ��
* ��ڲ�������ǰʱ�任������������1970����ǰ��������
* ���ڲ�������
* ��	ע��
****************************************************************************/
void SetCurrentTime(u32 CurrentTime_Seconds)
{
    LocalTime = CurrentTime_Seconds;
}
