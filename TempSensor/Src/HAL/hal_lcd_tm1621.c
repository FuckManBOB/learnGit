#include "hal_lcd_tm1621.h" 

char Tab0[14]= {0}; //����  

/********************�Ӹ�λд������*************************/
void Write_Data_H(char Data, char Cnt)	   //Data�ĸ�cntλд��TM1621C����λ��ǰ
{
 char i;
 for(i=0;i<Cnt;i++)
 {
  LCD_WRITE_CLR;
  if(Data & 0x80) 	                          //�����λ����
    LCD_DATA_SET;		
  else 
    LCD_DATA_CLR;
  nop;
  nop;
  LCD_WRITE_SET;
  Data<<=1;
 } 
 LCD_WRITE_CLR;
 LCD_DATA_CLR;
}

/********************�ӵ�λд������*************************/
void Write_Data_L(char Data,char Cnt)          //Data �ĵ�cntλд��TM1621C����λ��ǰ 
{ 
 unsigned char i; 
 for(i=0;i<Cnt;i++) 
 { 
   LCD_WRITE_CLR;
   if(Data&0x01) 	                       //�ӵ�λ����
    LCD_DATA_SET;  
   else 
    LCD_DATA_CLR; 
   nop;
   nop;
   LCD_WRITE_SET;
   Data>>=1;
 }
 LCD_WRITE_CLR;
 LCD_DATA_CLR; 
} 

/********************д���������*************************/
void WriteCmd(char Cmd)
{
 LCD_CS_CLR;
 nop;
 Write_Data_H(0x80,4);     //д�������־100
 Write_Data_H(Cmd,8);      //д����������
 LCD_CS_SET;
 nop;
}

/*********ָ����ַд�����ݣ�ʵ��д���4λ************/
void WriteOneData(char Addr, char Data)
{
 LCD_CS_CLR;
 Write_Data_H(0xa0,3);     //д�����ݱ�־101
 Write_Data_H(Addr<<2,6);  //д���ַ����
 Write_Data_L(Data,4);     //д������
 LCD_CS_SET;
 nop;
}

/*********����д�뷽ʽ��ÿ������Ϊ8λ��д������************/

void WriteAllDataclr(char Addr,char *p,char byteCnt)
{
 char i;
 LCD_CS_CLR;
 Write_Data_H(0xa0,3);          //д�����ݱ�־101
 Write_Data_H(Addr<<2,6);	//д���ַ����  pin
 for(i=0;i<byteCnt;i++)		//д������
 {
   Write_Data_L(*p,8);	    
   p++;
 }
 LCD_CS_SET;
 nop;
}


//����PIN��COM�Ľ����
void WriteAllData(char Addr,char p)
{
 LCD_CS_CLR;
 Write_Data_H(0xa0,3);          //д�����ݱ�־101
 Write_Data_H(Addr<<2,6);	//д���ַ����  pin
 Write_Data_L(p,8);	    
 LCD_CS_SET;
 nop;
}

/*******************TM1621C��ʼ��**********************/
void TM1621C_init()
{
 P4DIR |= BIT3 + BIT4 + BIT5;
 LCD_CS_SET;
 LCD_WRITE_SET;
 LCD_DATA_SET;
 nop;
 //delay_ms(2);			
 WriteCmd(BIAS);		 //1/3ƫѹ 4������
 WriteCmd(RC);			 //�ڲ�RC��
 WriteCmd(SYSDIS);		 //��ϵͳ������LCDƫѹ������
 WriteCmd(SYSEN);		 //��ϵͳ����
 WriteCmd(LCDON);		 //��LCDƫѹ
}

/************************************************************************************
* ��    �ƣ�LCD_DisplayClr(void)
* ��    �ܣ�����
* ��ڲ�������
* ���ڲ�������
* ˵    ������
* ��    ע����
************************************************************************************/
void LcdClean(void)
{
  TM1621C_init();		        //LCD��ʼ��
  WriteAllDataclr(0,Tab0,14);
}

/************************************************************************************
* ��    �ƣ�void LCD_POWER(unsigned char PowerValue)
* ��    �ܣ�������ʾ
* ��ڲ�������Ҫ��ʾ�ĵ�������
* ���ڲ�������
* ˵    ������ڲ����ɺ궨�壺Ϊ��LCD_POWER_FULL��  ��LCD_POWER_FULL_REDUCE��
*                             ��LCD_POWER_FULL_MORE_REDUCE�� ��LCD_POWER_EMPTY��
*                             �ֱ��ʾΪ���������� ������һ������� 
*                             ��������������� ���յ�����
* ��    ע��������ʹ��3��4��5��λ��ʾ��ֵ5��λ��С����
************************************************************************************/
#if 0
void LCD_POWER(unsigned char PowerValue)
{
    TM1621C_init();		           //LCD��ʼ��
    
    WriteAllData(0x19,0x10);               //��ؿտ��ͼ
    switch(PowerValue)
    {
      case LCD_POWER_FULL:           
           WriteAllData(0x00,0x0F);        //���������
        break;
      case LCD_POWER_FULL_REDUCE:
           WriteAllData(0x00,0x07);        //��ؼ���һ�����
        break;
      case LCD_POWER_FULL_MORE_REDUCE:
           WriteAllData(0x00,0x03);        //��ؼ����������
        break;
      case LCD_POWER_FULL_MUCH_MORE_REDUCE:
           WriteAllData(0x00,0x01);        //��ؼ����������
      default:
        break;
    }
}
#endif
/************************************************************************************
* ��    �ƣ�void LCD_POWER(unsigned char PowerValue)
* ��    �ܣ�������ʾ
* ��ڲ�������Ҫ��ʾ�ĵ�������
* ���ڲ�������
* ˵    ������ڲ����ɺ궨�壺Ϊ��LCD_POWER_FULL��  ��LCD_POWER_FULL_REDUCE��
*                             ��LCD_POWER_FULL_MORE_REDUCE�� ��LCD_POWER_EMPTY��
*                             �ֱ��ʾΪ���������� ������һ������� 
*                             ��������������� ���յ�����
*                             Ϊ��LCD_SIGNAL_FULL��  ��LCD_SIGNAL_FULL_REDUCE��
*                             ��LCD_SIGNAL_FULL_MORE_REDUCE�� ��LCD_SIGNAL_EMPTY��
*                             �ֱ��ʾΪ�����źš� ������һ���źš� 
*                             �����������źš� �����źš�
* ��    ע��������ʹ��3��4��5��λ��ʾ��ֵ5��λ��С����
************************************************************************************/
#if 0
void LCD_SIGNAL(unsigned char SignalValue)
{
    TM1621C_init();		           //LCD��ʼ��
    
    WriteAllData(0x19,0x10);               //��ؿտ��ͼ
    
    switch(SignalValue)
    {
      case LCD_SIGNAL_FULL:           
           WriteAllData(0x00,0xF0);        //�ź���
        break;
      case LCD_SIGNAL_FULL_REDUCE:
           WriteAllData(0x00,0xE0);        //�źż���һ��
        break;
      case LCD_SIGNAL_FULL_MORE_REDUCE:
           WriteAllData(0x00,0xC0);        //�źż�������
        break;
      case LCD_SIGNAL_EMPTY:               //�źſ�
           WriteAllData(0x00,0x80);
        break;
      default:
        break;
    }
}
#endif

/************************************************************************************
* ��    �ƣ�void LCD_SIGNAL_POWER(unsigned char SignalValue, unsigned char PowerValue)
* ��    �ܣ��źŵ�����ʾ
* ��ڲ�������Ҫ��ʾ���źź͵�������
* ���ڲ�������
* ˵    ������ڲ����ɺ궨�壺Ϊ��LCD_POWER_FULL��  ��LCD_POWER_FULL_REDUCE��
*                             ��LCD_POWER_FULL_MORE_REDUCE�� ��LCD_POWER_EMPTY��
*                             �ֱ��ʾΪ���������� ������һ������� 
*                             ��������������� ���յ�����
* ��    ע��������ʹ��3��4��5��λ��ʾ��ֵ5��λ��С����
************************************************************************************/
void LCD_SIGNAL_POWER(unsigned char SignalValue, unsigned char PowerValue)
{
    TM1621C_init();		           //LCD��ʼ��
    
    WriteAllData(0x19,0x10);               //��ؿտ��ͼ
    
    switch(SignalValue)
    {
        case LCD_SIGNAL_FULL:             //�ź���
           switch(PowerValue)
           {
            case LCD_POWER_FULL:           
                 WriteAllData(0x00,LCD_SIGNAL_FULL_BIT | LCD_POWER_FULL_BIT); //���������
                 return;
            case LCD_POWER_FULL_REDUCE:
                 WriteAllData(0x00,LCD_SIGNAL_FULL_BIT | LCD_POWER_FULL_REDUCE_BIT);        //��ؼ���һ�����
                 return;
            case LCD_POWER_FULL_MORE_REDUCE:
                 WriteAllData(0x00,LCD_SIGNAL_FULL_BIT | LCD_POWER_FULL_MORE_REDUCE_BIT);  //��ؼ����������
                 return;
            case LCD_POWER_FULL_MUCH_MORE_REDUCE:
                 WriteAllData(0x00,LCD_SIGNAL_FULL_BIT | LCD_POWER_FULL_MUCH_MORE_REDUCE_BIT); //��ؼ����������
                 return;
            default:
                 return;
           }
        break;
        case LCD_SIGNAL_FULL_REDUCE:                      //�źż���һ��
           switch(PowerValue)
           {
            case LCD_POWER_FULL:           
                 WriteAllData(0x00,LCD_SIGNAL_FULL_REDUCE_BIT | LCD_POWER_FULL_BIT); //���������
                 return;
            case LCD_POWER_FULL_REDUCE:
                 WriteAllData(0x00,LCD_SIGNAL_FULL_REDUCE_BIT | LCD_POWER_FULL_REDUCE_BIT);        //��ؼ���һ�����
                 return;
            case LCD_POWER_FULL_MORE_REDUCE:
                 WriteAllData(0x00,LCD_SIGNAL_FULL_REDUCE_BIT | LCD_POWER_FULL_MORE_REDUCE_BIT);  //��ؼ����������
                 return;
            case LCD_POWER_FULL_MUCH_MORE_REDUCE:
                 WriteAllData(0x00,LCD_SIGNAL_FULL_REDUCE_BIT | LCD_POWER_FULL_MUCH_MORE_REDUCE_BIT); //��ؼ����������
                 return;
            default:
                 return;
           }        
        break;
        case LCD_SIGNAL_FULL_MORE_REDUCE:                 //�źż�������
           switch(PowerValue)
           {
            case LCD_POWER_FULL:           
                 WriteAllData(0x00,LCD_SIGNAL_FULL_MORE_REDUCE_BIT | LCD_POWER_FULL_BIT); //���������
                 return;
            case LCD_POWER_FULL_REDUCE:
                 WriteAllData(0x00,LCD_SIGNAL_FULL_MORE_REDUCE_BIT | LCD_POWER_FULL_REDUCE_BIT);        //��ؼ���һ�����
                 return;
            case LCD_POWER_FULL_MORE_REDUCE:
                 WriteAllData(0x00,LCD_SIGNAL_FULL_MORE_REDUCE_BIT | LCD_POWER_FULL_MORE_REDUCE_BIT);  //��ؼ����������
                 return;
            case LCD_POWER_FULL_MUCH_MORE_REDUCE:
                 WriteAllData(0x00,LCD_SIGNAL_FULL_MORE_REDUCE_BIT | LCD_POWER_FULL_MUCH_MORE_REDUCE_BIT); //��ؼ����������
                 return;
            default:
                 return;
           }        
        break;
        case LCD_SIGNAL_EMPTY:                            //�źſ�
           switch(PowerValue)
           {
            case LCD_POWER_FULL:           
                 WriteAllData(0x00,LCD_SIGNAL_EMPTY_BIT | LCD_POWER_FULL_BIT); //���������
                 return;
            case LCD_POWER_FULL_REDUCE:
                 WriteAllData(0x00,LCD_SIGNAL_EMPTY_BIT | LCD_POWER_FULL_REDUCE_BIT);        //��ؼ���һ�����
                 return;
            case LCD_POWER_FULL_MORE_REDUCE:
                 WriteAllData(0x00,LCD_SIGNAL_EMPTY_BIT | LCD_POWER_FULL_MORE_REDUCE_BIT);  //��ؼ����������
                 return;
            case LCD_POWER_FULL_MUCH_MORE_REDUCE:
                 WriteAllData(0x00,LCD_SIGNAL_EMPTY_BIT | LCD_POWER_FULL_MUCH_MORE_REDUCE_BIT); //��ؼ����������
                 return;
            default:
                 return;
           }
        break;
      default:
        break;
    }    
}

/************************************************************************************
* ��    �ƣ�void LcdNegNumber(short int Value)
* ��    �ܣ���ֵ��ʾ
* ��ڲ�������Ҫ��ʾ����ֵ
* ���ڲ�������
* ˵    ����TM1621C LCD�������������Ƶ����LCDҺ����6�����ֺ�5��С�����Լ�������
*           �ź��� ������ʾ��
*                               8.  8.  8.  8.  8.  8           
*                               |   |   |   |   |   |           ����������1-6������8
*                              1�� 2�� 3�� 4�� 5�� 6��
* ��    ע��������ʹ��3��4��5��λ��ʾ��ֵ5��λ��С����
************************************************************************************/
void LcdPosNumber(int Value)
{
    unsigned int j;

    unsigned int dividedData[3];
    TM1621C_init();		//LCD��ʼ��
                          
    dividedData[0]=Value/100;                   //��λ
    dividedData[1]=Value%100/10;                //ʮλ
    dividedData[2]=Value%10;                    //��λ
    
    /*������ʾ���۽����ֶ�����Һ����������λ�ü����ִ�ʮλ��������*/
    LCD_Number(WANWEI,  POSITIVE);               //���� 
    LCD_Number(SHIWEI,  dividedData[2]);         //ʮλ
    LCD_Number(BAIWEI,  dividedData[1]);         //��λ
    LCD_Number(QIANWEI, dividedData[0]);         //ǧλ
    for (j =0;j<=800;j++);   
}

/************************************************************************************
* ��    �ƣ�void LcdNegNumber(short int Value)
* ��    �ܣ���ֵ��ʾ
* ��ڲ�������Ҫ��ʾ����ֵ
* ���ڲ�������
* ˵    ����TM1621C LCD�������������Ƶ����LCDҺ����6�����ֺ�5��С�����Լ�������
*           �ź��� ������ʾ��
*                               8.  8.  8.  8.  8.  8           
*                               |   |   |   |   |   |           ����������1-6������8
*                              1�� 2�� 3�� 4�� 5�� 6��
* ��    ע��������ʹ��2��λ��ʾ����3��4��5��λ��ʾ��ֵ5��λ��С����
************************************************************************************/
void LcdNegNumber(int Value)
{
    unsigned int j;

    unsigned int dividedData[3];
    TM1621C_init();		                //LCD��ʼ��
                          
    dividedData[0]=Value/100;                   //��λ
    dividedData[1]=Value%100/10;                //ʮλ
    dividedData[2]=Value%10;                    //��λ
    
    /*������ʾ���۽����ֶ�����Һ����������λ�ü����ִ�ʮλ��������*/
    LCD_Number(WANWEI,  NEGATIVE);              //���� 
    LCD_Number(SHIWEI,  dividedData[2]);        //ʮλ
    LCD_Number(BAIWEI,  dividedData[1]);        //��λ
    LCD_Number(QIANWEI, dividedData[0]);        //ǧλ
    for (j =0;j<=800;j++);   
}

/************************************************************************************
* ��    �ƣ�void LCD_Number(unsigned char position, unsigned char number)
* ��    �ܣ���ʾ������
* ��ڲ���1��position �����º궨�� ��GEWEI��  ��SHIWEI�� ��BAIWEI�� ��QIANWEI�� ��WANWEI��
* ��ڲ���2��number  ��Ҫ��ʾ��������
* ���ڲ�������
* ��    ע��1.�����ｫ��λ��case 10��Ϊ����λʹ��
*           2.�����ｫʮλ����С���㣬Ϊ�˵�ǰ�豸�ܹ�����һλС��
* ˵    ����TM1621C LCD�������������Ƶ����LCDҺ����6�����ֺ�5��С�����Լ�������
*           �ź��� ������ʾ��
*                               8.  8.  8.  8.  8.  8           
*                               |   |   |   |   |   |           ����������1-6������8
*                              1�� 2�� 3�� 4�� 5�� 6��
************************************************************************************/
void LCD_Number(unsigned char position, unsigned char number)
{  
  switch (position)//��������
  {
#if 0
    case GEWEI:	  
        switch(number)	  //��λ ->�ֲ���6��λ����
        {
              case 0: WriteAllData(GEWEI,0xAF);
              break;
              case 1: WriteAllData(GEWEI,0x06);
              break;
              case 2: WriteAllData(GEWEI,0x6D);
              break;
              case 3: WriteAllData(GEWEI,0x4F);
              break;
              case 4: WriteAllData(GEWEI,0xC6);
              break;
              case 5: WriteAllData(GEWEI,0xCB);
              break;
              case 6: WriteAllData(GEWEI,0xEB);
              break;
              case 7: WriteAllData(GEWEI,0x0E);
              break;
              case 8: WriteAllData(GEWEI,0xEF);
              break;
              case 9: WriteAllData(GEWEI,0xCF);
              break;
        }
        break;
#endif
    case SHIWEI:	        
         switch(number)   //ʮλ ->�ֲ���5��λ����
        {
              case 0: WriteAllData(SHIWEI,0xBF);        //����С���㽫ʮ�����Ƶĸ���λ��һ
              break;
              case 1: WriteAllData(SHIWEI,0x16);
              break;
              case 2: WriteAllData(SHIWEI,0x7D);
              break;
              case 3: WriteAllData(SHIWEI,0x5F);
              break;
              case 4: WriteAllData(SHIWEI,0xD6);
              break;
              case 5: WriteAllData(SHIWEI,0xDB);
              break;
              case 6: WriteAllData(SHIWEI,0xFB);
              break;
              case 7: WriteAllData(SHIWEI,0x1E);
              break;
              case 8: WriteAllData(SHIWEI,0xFF);
              break;
              case 9: WriteAllData(SHIWEI,0xDF);
              break;
        }
        break;
    case BAIWEI:	
        switch(number)	  //��λ ->�ֲ���4��λ����
        {
            case 0: WriteAllData(BAIWEI,0xAF);
            break;
            case 1: WriteAllData(BAIWEI,0x06);
            break;
            case 2: WriteAllData(BAIWEI,0x6D);
            break;
            case 3: WriteAllData(BAIWEI,0x4F);
            break;
            case 4: WriteAllData(BAIWEI,0xC6);
            break;
            case 5: WriteAllData(BAIWEI,0xCB);
            break;
            case 6: WriteAllData(BAIWEI,0xEB);
            break;
            case 7: WriteAllData(BAIWEI,0x0E);
            break;
            case 8: WriteAllData(BAIWEI,0xEF);
            break;
            case 9: WriteAllData(BAIWEI,0xCF);
            break;
        }
        break;
    case QIANWEI:	
        switch(number)	 //ǧλ ->�ֲ���3��λ����
        {
            case 0: WriteAllData(QIANWEI,0xAF);
            break;
            case 1: WriteAllData(QIANWEI,0x06);
            break;
            case 2: WriteAllData(QIANWEI,0x6D);
            break;
            case 3: WriteAllData(QIANWEI,0x4F);
            break;
            case 4: WriteAllData(QIANWEI,0xC6);
            break;
            case 5: WriteAllData(QIANWEI,0xCB);
            break;
            case 6: WriteAllData(QIANWEI,0xEB);
            break;
            case 7: WriteAllData(QIANWEI,0x0E);
            break;
            case 8: WriteAllData(QIANWEI,0xEF);
            break;
            case 9: WriteAllData(QIANWEI,0xCF);
            break;
        }
        break;
    case WANWEI:	
         switch(number)   //��λ ->�ֲ���2��λ����(ע��ʮ��λʡ��)
         {
#if 0
            case 0: WriteAllData(WANWEI,0xAF);
            break;
            case 1: WriteAllData(WANWEI,0x06);
            break;
            case 2: WriteAllData(WANWEI,0x6D);
            break;
            case 3: WriteAllData(WANWEI,0x4F);
            break;
            case 4: WriteAllData(WANWEI,0xC6);
            break;
            case 5: WriteAllData(WANWEI,0xCB);
            break;
            case 6: WriteAllData(WANWEI,0xEB);
            break;
            case 7: WriteAllData(WANWEI,0x0E);
            break;
            case 8: WriteAllData(WANWEI,0xEF);
            break;
            case 9: WriteAllData(WANWEI,0xCF);
            break;
#endif
            case NEGATIVE:
              WriteAllData(WANWEI,0x40);          //����λ
            break;
            case POSITIVE:
              WriteAllData(WANWEI,0x00);          //����λ
            break;
        }
        break;    
  }
}


/************************��������Battery��Rf************************/

/*
*       ��ص�����ʾ
*/
#if 0
void Power_Lcd_Display(void)
{
    float BatteryTemp = 0.0;
    
    BatteryTemp  = Battery_Value_Get();                 //��Դģ���ѹ
    
    if( BatteryTemp > 3.0 )
      LCD_POWER(LCD_POWER_FULL);
    else if( BatteryTemp > 2.7 && BatteryTemp <= 3.0 )
      LCD_POWER(LCD_POWER_FULL_REDUCE);
    else if( BatteryTemp > 2.4 && BatteryTemp <= 2.7 )
      LCD_POWER(LCD_POWER_FULL_MORE_REDUCE);
    else if( BatteryTemp > 2.2 && BatteryTemp <= 2.4 )
      LCD_POWER(LCD_POWER_FULL_MUCH_MORE_REDUCE);
    else
      LCD_POWER(LCD_POWER_EMPTY);
}    

/*
*       �ź���ʾ
*/
void Signal_Lcd_Display(void)
{
    int RfEnergyTemp = 0;

    
    CMD_MODE_ON;                //�����������ģʽ
    RfEnergyTemp = RF_RSSI_Get();
    TC_MODE_ON;                 //����͸������ģʽ
    
    if(RfEnergyTemp > -40 && RfEnergyTemp <= 20)
       LCD_SIGNAL(LCD_SIGNAL_FULL);                     //�ź�����
    else if(RfEnergyTemp > -64 && RfEnergyTemp <= -40)
       LCD_SIGNAL(LCD_SIGNAL_FULL_REDUCE);              //�ź�2��
    else if(RfEnergyTemp > -80 && RfEnergyTemp <= -64)
       LCD_SIGNAL(LCD_SIGNAL_FULL_MORE_REDUCE);         //�ź�1��
    else if(RfEnergyTemp > -87 && RfEnergyTemp <= -80)
       LCD_SIGNAL(LCD_SIGNAL_EMPTY);                    //���ź�
  
}

#endif

/*
*       �������źż����ʾ
*/
void Symbol_Lcd_Display(void)
{  
    static u8  SignalFlag    = 0;
    static u8  PowerFlag     = 0;
    
/* ���ڲ�׽F8913�źŴ���ģʽ�л�����Ӧ���ȶ�Ӱ��������Ҽ����ź��� */    
#if RSSI_TEST     //ʹ�����ź�
    static int RfEnergyTemp  = 0;
   
    
    CMD_MODE_ON;                //�����������ģʽ
    RfEnergyTemp = RF_RSSI_Get();
    TC_MODE_ON;                 //����͸������ģʽ
    
    if(RfEnergyTemp > -40 && RfEnergyTemp <= 20)
       SignalFlag = LCD_SIGNAL_FULL;                     //�ź�����
    else if(RfEnergyTemp > -64 && RfEnergyTemp <= -40)
       SignalFlag = LCD_SIGNAL_FULL_REDUCE;              //�ź�2��
    else if(RfEnergyTemp > -80 && RfEnergyTemp <= -64)
       SignalFlag = LCD_SIGNAL_FULL_MORE_REDUCE;         //�ź�1��
    else if(RfEnergyTemp > -87 && RfEnergyTemp <= -80)
       SignalFlag = LCD_SIGNAL_EMPTY;                    //���ź�
    else if(RfEnergyTemp == 100)
       SignalFlag = LCD_SIGNAL_EMPTY;                    //���ڷ���100������Ϣ��ѡ����ź���Ϊ����
#else   //ʹ�ü��ź�
    SignalFlag = LCD_SIGNAL_FULL;
#endif

#if BATTERY_TEST
    static float BatteryTemp = 0.0;
    
    BatteryTemp  = Battery_Value_Get();                 //��Դģ���ѹ
    
    if( BatteryTemp > 3.2 )
      PowerFlag = LCD_POWER_FULL;                               //����
    else if( BatteryTemp > 3.1 && BatteryTemp <= 3.2 )
      PowerFlag = LCD_POWER_FULL_REDUCE;                        //3��
    else if( BatteryTemp > 2.8 && BatteryTemp <= 3.1 )
      PowerFlag = LCD_POWER_FULL_MORE_REDUCE;                   //2��
    else if( BatteryTemp <= 2.8 )
      PowerFlag = LCD_POWER_FULL_MUCH_MORE_REDUCE;              //1��
#else
    PowerFlag = LCD_POWER_FULL;
#endif    
    LCD_SIGNAL_POWER(SignalFlag, PowerFlag);
}