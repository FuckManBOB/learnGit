#include "hal_lcd.h"

#define LCD     1

unsigned short int number_buffer[3]={0,0,0};		//数字区域缓冲区分为3组
unsigned short int signal_buffer = 0;			//信号区缓冲区单组

/*------------------------------------------------------------------------------*/
/*			        HT1621驱动说明                                  */
/*	note：addr对应pin，data对应com，两者交点即所需点亮的段位		*/
/*      COM4-COM1 高位对高位 ,低位对低位					*/
/*	开始需要发送SYS_EN和LCD_ON两个命令，细节查看HT1621手册		        */
/*------------------------------------------------------------------------------*/

//命令步骤：1片选		 			数据模式：1.片选
//	    2模式					          2.写数据模式
//	    3命令数据				                  3.写地址
//	    4复位引脚状态			                  4.写数据
//							          5.复位引脚状态


void LcdIo_Init()
{
    P4DIR |= 0xD0;      //初始化LCD IO口BIT4 BIT6 BIT7输出，省略了输入IO BIT5未用到
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
    {   // address Τ 6 bit τ.
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
//  Procedure : 映射number_buffer缓存对应的数字区域显示的数字
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
	Write_Addr(0x0d);//将HT1621驱动RAM分割成两部分：此部分针对GDM0689液晶的数字段进行分界

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
//  Procedure : 映射signal_buffer缓存对应的信号图标区域显示的图标
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
	Write_Addr(0x07);//将HT1621驱动RAM分割成两部分：此部分针对GDM0689液晶的信号图标段进行分界

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
/*							     主要功能函数							        */
/*------------------------------------------------------------------------------*/
/*
* LCD命令初始化
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
* 发送命令
*/
void SendLcdCmd(u8 cmd)
{
	CS_LOW;
	CMD_Mode();
	Write_CMD(cmd);
	Test_LCD_Reset();
}

/*
* 清数字部分的屏 
*/
void LcdClr_Number(void)
{
	int i;

	for(i = 0; i < 3; i++)
		number_buffer[i] = 0;
	Lcd_Number_data();
}

/*
* 清信号图标部分的屏 
*/
void LcdClr_Signal(void)
{
	signal_buffer = 0;
	Lcd_Signal_data();
}

/*
* 清全屏 
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
*	描述：显示有负号，百位，十位，“点”，个位，物理符号				
*/
void lcd_number(u8 position, u8 number)
{
  switch (position)//数字区域
  {
    case 0:	  //符号位
        switch(number)	  //0代表正	1代表负
        {
                case 0: number_buffer[0] |= 0x0000;	 
                break;
                case 1: number_buffer[0] |= 0x0002;
                break;
        }
        break;
    case 1:	//百位
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
    case 2:	//十位
        switch(number)	   //note:8代表压力符号若去掉改为0	 带小数点
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
    case 3:	//个位
        switch(number)	 //note:8代表温度符号若去掉改为0
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

/*描述：电池电量划分为4个等级：
*		0为<5%，1为25%左右，
*		2为50%左右，3为75%左右，4为<100%
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

/*描述：网络信号划分为4个等级：
*		0为无连接，1为能量25%左右，
*		2为能量50%左右，3为80%左右
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

    lcd_number(0,i);					//对于温度的正负，i=0为正
    lcd_number(1,dividedData[0]);			//百位
    lcd_number(2,dividedData[1]);			//十位
    lcd_number(3,dividedData[2]);			//个位
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

    lcd_number(0,i);					//对于温度的正负，i=1为负
    lcd_number(1,dividedData[0]);		//百位
    lcd_number(2,dividedData[1]);		//十位
    lcd_number(3,dividedData[2]);		//个位
    Lcd_Number_data();
    for (j =0;j<=800;j++);   
}

void LcdPowerSignalState(u8 powerState,u8 signalState)//note:每次必须同时将电力信号和网络信号写入，否则或失去其中一个
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
*       上电清屏
*/
void LcdClean(void)
{
    LCD_Init();
    LcdClr_Number(); 
    LcdClr_All();
}

/************************以下来自Battery和Rf************************/

/*
*       电池电量显示
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
       BatteryFlag = 0;         //电量小于5%
    else if(BatteryTemp <= 960 && BatteryTemp > 350)
       BatteryFlag = 1;         //电量小于25%
    else if(BatteryTemp <= 1925 && BatteryTemp > 960)
       BatteryFlag = 2;         //电量小于50%
    else if(BatteryTemp <= 2560 && BatteryTemp > 1925)
       BatteryFlag = 3;         //电量小于80%
    else if(BatteryTemp <= 5000 && BatteryTemp > 2560)
       BatteryFlag = 4;         //电量小于100%
    
    if(RfEnergyTemp > -40 && RfEnergyTemp <= 20)
       RfEnergyFlag = 3;         //信号满格
    else if(RfEnergyTemp > -64 && RfEnergyTemp <= -40)
       RfEnergyFlag = 2;         //信号2格
    else if(RfEnergyTemp > -80 && RfEnergyTemp <= -64)
       RfEnergyFlag = 1;         //信号1格
    else if(RfEnergyTemp > -87 && RfEnergyTemp <= -80)
       RfEnergyFlag = 0;         //无信号
    else
       RfEnergyFlag = 0;
    
    /*
        有关RfEnergyFlag判断
    */
      LcdClr_Signal();          //显示前清除符号区域
      LcdPowerSignalState(BatteryFlag,RfEnergyFlag);
}
	

