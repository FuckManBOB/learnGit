#include "hal_lcd.h"

#define LCD     1

unsigned short int number_buffer[3]={0,0,0};		//�������򻺳�����Ϊ3��
unsigned short int signal_buffer = 0;			//�ź�������������

/*------------------------------------------------------------------------------*/
/*			        HT1621����˵��                                  */
/*	note��addr��Ӧpin��data��Ӧcom�����߽��㼴��������Ķ�λ		*/
/*      COM4-COM1 ��λ�Ը�λ ,��λ�Ե�λ					*/
/*	��ʼ��Ҫ����SYS_EN��LCD_ON�������ϸ�ڲ鿴HT1621�ֲ�		        */
/*------------------------------------------------------------------------------*/

//����裺1Ƭѡ		 			����ģʽ��1.Ƭѡ
//	    2ģʽ					          2.д����ģʽ
//	    3��������				                  3.д��ַ
//	    4��λ����״̬			                  4.д����
//							          5.��λ����״̬


void LcdIo_Init()
{
    P4DIR |= 0xD0;      //��ʼ��LCD IO��BIT4 BIT6 BIT7�����ʡ��������IO BIT5δ�õ�
    P4OUT |= 0xD0;
}

#if     LCD
//----------------------------------------------------------------------
//  Procedure : WRB_Clock
//  Function  : make a WRB pulse, let LCD driver latch data
//----------------------------------------------------------------------
void WRB_Clock()
{
    WR_LOW;
    WR_LOW;
    WR_LOW;
    WR_LOW;
    WR_HIGH;    // latch
} // WRB_Clock


//----------------------------------------------------------------------
//  Procedure : CMD_mode	  ID:100
//  Function  : Set command mode
//----------------------------------------------------------------------
void CMD_Mode()
{
    DATA_HIGH;
    WRB_Clock();
    DATA_LOW;
    WRB_Clock();
    DATA_LOW;
    WRB_Clock();
} // CMD_mode


//----------------------------------------------------------------------
//  Procedure : Write_mode		ID:101
//  Function  : set to write mode
//----------------------------------------------------------------------
void Write_Mode()
{
    DATA_HIGH;
    WRB_Clock();
    DATA_LOW;
    WRB_Clock();
    DATA_HIGH;
    WRB_Clock();
} // Write_mode


//----------------------------------------------------------------------
//  Procedure : Write_CMD
//  Function  : write a command to LCD driver
//----------------------------------------------------------------------
void Write_CMD(u8 cmd)
{
    u8 i;
    for (i = 0; i < 8; i ++)
    {
        if (cmd & 0x80)
            DATA_HIGH;
        else
            DATA_LOW;
        WRB_Clock();
        cmd <<= 1;
    }
    //Dummy command bit, but necessary
    //DATA_LOW;
    //WRB_Clock();
} // Write_CMD

#endif

//----------------------------------------------------------------------
//  Procedure : Write_Data
//  Function  : write 4 bit data to LCD
//----------------------------------------------------------------------
void Write_Data(u8 nibble)
{
    u8 i;

    for (i = 0; i < 4; i ++)
    {
        if (nibble & 0x01)
            DATA_HIGH;
        else
            DATA_LOW;
        WRB_Clock();
        nibble >>= 1;
    }
} // Write_Data


//----------------------------------------------------------------------
//  Procedure : Write_Addr
//  Function  : write address to LCD
//----------------------------------------------------------------------
void Write_Addr(u8 addr)
{
    u8 i;

    addr <<= 2;
    for (i = 0; i < 6; i ++)
    {   // address �� 6 bit �Ӥw.
        if (addr & 0x80)
            DATA_HIGH;
        else
            DATA_LOW;
        WRB_Clock();
        addr <<= 1;
    }
} // Write_Addr


//----------------------------------------------------------------------
//  Procedure : Test_LCD_Reset
//  Function  : Let LCD 4 signal to high
//----------------------------------------------------------------------
void Test_LCD_Reset()
{
    WR_LOW;
    WR_HIGH;
    DATA_HIGH;
    CS_HIGH;    
} 


//----------------------------------------------------------------------
//  Procedure : Write_3208_Data
//  Function  : write 3208 one kind data
//  Input  : nibble : start nibble value
//  Output : None
//----------------------------------------------------------------------
void Write_3208_Data(u8 nibble,u8 b,u8 addr)
{
    u8 i;

    CS_LOW;
    Write_Mode();
    Write_Addr(addr);

    for (i = 0; i < b; i ++)
    {   // 64 times
        Write_Data(nibble);
    }
    Test_LCD_Reset();
} // Write_3208_Data

//----------------------------------------------------------------------
//  Procedure : ӳ��number_buffer�����Ӧ������������ʾ������
//  Function  : void Lcd_Number_data(void)
//  Input  : None
//  Output : None
//----------------------------------------------------------------------
void Lcd_Number_data(void)
{
	u8 i,j;
	unsigned short int temp_data;

	CS_LOW;
	Write_Mode();
	Write_Addr(0x0d);//��HT1621����RAM�ָ�������֣��˲������GDM0689Һ�������ֶν��зֽ�

	for(i = 0; i < 3; i++)
	{
		temp_data = number_buffer[i];
		for(j = 0; j < 16; j++)
		{
			if (temp_data & 0x01)
				DATA_HIGH;
			else
				DATA_LOW;
			WRB_Clock();
			temp_data >>= 1;
		}
	}
	Test_LCD_Reset();
}

//----------------------------------------------------------------------
//  Procedure : ӳ��signal_buffer�����Ӧ���ź�ͼ��������ʾ��ͼ��
//  Function  : void Lcd_Signal_data()
//  Input  : None
//  Output : None
//----------------------------------------------------------------------
void Lcd_Signal_data()
{
	u8 j;
	unsigned short int temp_data;

	CS_LOW;
	Write_Mode();
	Write_Addr(0x07);//��HT1621����RAM�ָ�������֣��˲������GDM0689Һ�����ź�ͼ��ν��зֽ�

	temp_data = signal_buffer;
	for(j = 0; j < 16; j++)
	{
		if (temp_data & 0x01)
			DATA_HIGH;
		else
			DATA_LOW;
		WRB_Clock();
		temp_data >>= 1;
	}
	Test_LCD_Reset();
}
/*------------------------------------------------------------------------------*/
/*							     ��Ҫ���ܺ���							        */
/*------------------------------------------------------------------------------*/
/*
* LCD�����ʼ��
*/
void LCD_Init(void)
{
	LcdIo_Init();
        Test_LCD_Reset();
	SendLcdCmd(SYS_EN);
	SendLcdCmd(LCD_ON);
	SendLcdCmd(LCD_1_3BIAS1_4DUTY);
}

/*
* ��������
*/
void SendLcdCmd(u8 cmd)
{
	CS_LOW;
	CMD_Mode();
	Write_CMD(cmd);
	Test_LCD_Reset();
}

/*
* �����ֲ��ֵ��� 
*/
void LcdClr_Number(void)
{
	int i;

	for(i = 0; i < 3; i++)
		number_buffer[i] = 0;
	Lcd_Number_data();
}

/*
* ���ź�ͼ�겿�ֵ��� 
*/
void LcdClr_Signal(void)
{
	signal_buffer = 0;
	Lcd_Signal_data();
}

/*
* ��ȫ�� 
*/
void LcdClr_All(void)
{
	u8 i,j;
	u8 temp_data = 0;

	CS_LOW;
	Write_Mode();
	Write_Addr(0x00);
	
	for(i = 0; i < 32; i++)
	{
		for(j = 0; j < 4; j++)
		{
			if (temp_data & 0x01)
				DATA_HIGH;
			else
				DATA_LOW;
			WRB_Clock();
			temp_data >>= 1;
		}
	}
	Test_LCD_Reset();
}


/*
*	��������ʾ�и��ţ���λ��ʮλ�����㡱����λ���������				
*/
void lcd_number(u8 position, u8 number)
{
  switch (position)//��������
  {
    case 0:	  //����λ
        switch(number)	  //0������	1����
        {
                case 0: number_buffer[0] |= 0x0000;	 
                break;
                case 1: number_buffer[0] |= 0x0002;
                break;
        }
        break;
    case 1:	//��λ
         switch(number)
        {
                case 0: number_buffer[0] |= 0x7d00;
                break;
                case 1: number_buffer[0] |= 0x6000;
                break;
                case 2: number_buffer[0] |= 0x3e00;
                break;
                case 3: number_buffer[0] |= 0x7a00;
                break;
                case 4: number_buffer[0] |= 0x6300;
                break;
                case 5: number_buffer[0] |= 0x5b00;
                break;
                case 6: number_buffer[0] |= 0x5f00;
                break;
                case 7: number_buffer[0] |= 0x7000;
                break;
                case 8: number_buffer[0] |= 0x7f00;
                break;
                case 9: number_buffer[0] |= 0x7b00;
                break;
        }
        break;
    case 2:	//ʮλ
        switch(number)	   //note:8����ѹ��������ȥ����Ϊ0	 ��С����
        {
            case 0: number_buffer[1] |= 0xfd00;
            break;
            case 1: number_buffer[1] |= 0xe000;
            break;
            case 2: number_buffer[1] |= 0xbe00;
            break;
            case 3: number_buffer[1] |= 0xfa00;
            break;
            case 4: number_buffer[1] |= 0xe300;
            break;
            case 5: number_buffer[1] |= 0xdb00;
            break;
            case 6: number_buffer[1] |= 0xdf00;
            break;
            case 7: number_buffer[1] |= 0xf000;
            break;
            case 8: number_buffer[1] |= 0xff00;
            break;
            case 9: number_buffer[1] |= 0xfb00;
            break;
        }
        break;
    case 3:	//��λ
        switch(number)	 //note:8�����¶ȷ�����ȥ����Ϊ0
        {
            case 0: number_buffer[2] |= 0x807d;
            break;
            case 1: number_buffer[2] |= 0x8060;
            break;
            case 2: number_buffer[2] |= 0x803e;
            break;
            case 3: number_buffer[2] |= 0x807a;
            break;
            case 4: number_buffer[2] |= 0x8063;
            break;
            case 5: number_buffer[2] |= 0x805b;
            break;
            case 6: number_buffer[2] |= 0x805f;
            break;
            case 7: number_buffer[2] |= 0x8070;
            break;
            case 8: number_buffer[2] |= 0x807f;
            break;
            case 9: number_buffer[2] |= 0x807b;
            break;
        }
        break;
  }
}

/*��������ص�������Ϊ4���ȼ���
*		0Ϊ<5%��1Ϊ25%���ң�
*		2Ϊ50%���ң�3Ϊ75%���ң�4Ϊ<100%
*/
void power_state(u8 state)
{
  switch(state)
  {
      case 0: signal_buffer |= 0x0080;
                      break;
      case 1: signal_buffer |= 0x0880;
                      break;
      case 2: signal_buffer |= 0x0c80;
                      break;
      case 3: signal_buffer |= 0x0e80;
                      break;
      case 4: signal_buffer |= 0x0f80;
                      break;
  }
}

/*�����������źŻ���Ϊ4���ȼ���
*		0Ϊ�����ӣ�1Ϊ����25%���ң�
*		2Ϊ����50%���ң�3Ϊ80%����
*/
void signal_state(u8 state)
{
   switch(state)
  {
      case 0: signal_buffer |= 0x1000;
                      break;
      case 1: signal_buffer |= 0x3000;
                      break;
      case 2: signal_buffer |= 0x7000;
                      break;
      case 3: signal_buffer |= 0xf000;
                      break;
  }
}

void LcdPosNumber(short int Temperature)
{
    int i;
    unsigned int j;

    unsigned short int dividedData[3];
    LCD_Init();
    LcdClr_Number(); 
                          
    dividedData[0]=Temperature/100;
    dividedData[1]=Temperature%100/10;
    dividedData[2]=Temperature%10;

    i=0;

    lcd_number(0,i);					//�����¶ȵ�������i=0Ϊ��
    lcd_number(1,dividedData[0]);			//��λ
    lcd_number(2,dividedData[1]);			//ʮλ
    lcd_number(3,dividedData[2]);			//��λ
    Lcd_Number_data();
    for (j =0;j<=800;j++);   
}
void LcdNegNumber(short int Temperature)
{
    int i;
    unsigned int j;

    unsigned short int dividedData[3];
    LcdIo_Init();
    LCD_Init();
    LcdClr_Number();	
                            
    dividedData[0]=Temperature/100;
    dividedData[1]=Temperature%100/10;
    dividedData[2]=Temperature%10;

    i=1;

    lcd_number(0,i);					//�����¶ȵ�������i=1Ϊ��
    lcd_number(1,dividedData[0]);		//��λ
    lcd_number(2,dividedData[1]);		//ʮλ
    lcd_number(3,dividedData[2]);		//��λ
    Lcd_Number_data();
    for (j =0;j<=800;j++);   
}

void LcdPowerSignalState(u8 powerState,u8 signalState)//note:ÿ�α���ͬʱ�������źź������ź�д�룬�����ʧȥ����һ��
{
    unsigned int j;
    LcdIo_Init();
    LCD_Init();
    LcdClr_Signal();	
                            
    power_state(powerState);
    signal_state(signalState);
    Lcd_Signal_data();
    for (j =0;j<=800;j++);   
}

/*
*       �ϵ�����
*/
void LcdClean(void)
{
    LCD_Init();
    LcdClr_Number(); 
    LcdClr_All();
}

/************************��������Battery��Rf************************/

/*
*       ��ص�����ʾ
*/
void Symbol_Lcd_Display(void)
{
    u8  BatteryFlag = 4;
    u8  RfEnergyFlag = 0;
    u32 BatteryTemp = 0;
    int RfEnergyTemp = 0;
    
    BatteryTemp = Battery_Adc_Get();
    RfEnergyTemp = RF_RSSI_Get();
    
    if(BatteryTemp <= 350)
       BatteryFlag = 0;         //����С��5%
    else if(BatteryTemp <= 960 && BatteryTemp > 350)
       BatteryFlag = 1;         //����С��25%
    else if(BatteryTemp <= 1925 && BatteryTemp > 960)
       BatteryFlag = 2;         //����С��50%
    else if(BatteryTemp <= 2560 && BatteryTemp > 1925)
       BatteryFlag = 3;         //����С��80%
    else if(BatteryTemp <= 5000 && BatteryTemp > 2560)
       BatteryFlag = 4;         //����С��100%
    
    if(RfEnergyTemp > -40 && RfEnergyTemp <= 20)
       RfEnergyFlag = 3;         //�ź�����
    else if(RfEnergyTemp > -64 && RfEnergyTemp <= -40)
       RfEnergyFlag = 2;         //�ź�2��
    else if(RfEnergyTemp > -80 && RfEnergyTemp <= -64)
       RfEnergyFlag = 1;         //�ź�1��
    else if(RfEnergyTemp > -87 && RfEnergyTemp <= -80)
       RfEnergyFlag = 0;         //���ź�
    else
       RfEnergyFlag = 0;
    
    /*
        �й�RfEnergyFlag�ж�
    */
      LcdClr_Signal();          //��ʾǰ�����������
      LcdPowerSignalState(BatteryFlag,RfEnergyFlag);
}
	

