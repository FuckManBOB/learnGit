#include "hal_lcd_tm1621.h" 

char Tab0[14]= {0}; //清屏  

/********************从高位写入数据*************************/
void Write_Data_H(char Data, char Cnt)	   //Data的高cnt位写入TM1621C，高位在前
{
 char i;
 for(i=0;i<Cnt;i++)
 {
  LCD_WRITE_CLR;
  if(Data & 0x80) 	                          //从最高位发送
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

/********************从低位写入数据*************************/
void Write_Data_L(char Data,char Cnt)          //Data 的低cnt位写入TM1621C，低位在前 
{ 
 unsigned char i; 
 for(i=0;i<Cnt;i++) 
 { 
   LCD_WRITE_CLR;
   if(Data&0x01) 	                       //从低位发送
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

/********************写入控制命令*************************/
void WriteCmd(char Cmd)
{
 LCD_CS_CLR;
 nop;
 Write_Data_H(0x80,4);     //写入命令标志100
 Write_Data_H(Cmd,8);      //写入命令数据
 LCD_CS_SET;
 nop;
}

/*********指定地址写入数据，实际写入后4位************/
void WriteOneData(char Addr, char Data)
{
 LCD_CS_CLR;
 Write_Data_H(0xa0,3);     //写入数据标志101
 Write_Data_H(Addr<<2,6);  //写入地址数据
 Write_Data_L(Data,4);     //写入数据
 LCD_CS_SET;
 nop;
}

/*********连续写入方式，每次数据为8位，写入数据************/

void WriteAllDataclr(char Addr,char *p,char byteCnt)
{
 char i;
 LCD_CS_CLR;
 Write_Data_H(0xa0,3);          //写入数据标志101
 Write_Data_H(Addr<<2,6);	//写入地址数据  pin
 for(i=0;i<byteCnt;i++)		//写入数据
 {
   Write_Data_L(*p,8);	    
   p++;
 }
 LCD_CS_SET;
 nop;
}


//用于PIN和COM的交叉点
void WriteAllData(char Addr,char p)
{
 LCD_CS_CLR;
 Write_Data_H(0xa0,3);          //写入数据标志101
 Write_Data_H(Addr<<2,6);	//写入地址数据  pin
 Write_Data_L(p,8);	    
 LCD_CS_SET;
 nop;
}

/*******************TM1621C初始化**********************/
void TM1621C_init()
{
 P4DIR |= BIT3 + BIT4 + BIT5;
 LCD_CS_SET;
 LCD_WRITE_SET;
 LCD_DATA_SET;
 nop;
 //delay_ms(2);			
 WriteCmd(BIAS);		 //1/3偏压 4公共口
 WriteCmd(RC);			 //内部RC振荡
 WriteCmd(SYSDIS);		 //关系统振荡器和LCD偏压发生器
 WriteCmd(SYSEN);		 //打开系统振荡器
 WriteCmd(LCDON);		 //开LCD偏压
}

/************************************************************************************
* 名    称：LCD_DisplayClr(void)
* 功    能：清屏
* 入口参数：无
* 出口参数：无
* 说    明：无
* 备    注：无
************************************************************************************/
void LcdClean(void)
{
  TM1621C_init();		        //LCD初始化
  WriteAllDataclr(0,Tab0,14);
}

/************************************************************************************
* 名    称：void LCD_POWER(unsigned char PowerValue)
* 功    能：电量显示
* 入口参数：需要显示的电量容量
* 出口参数：无
* 说    明：入口参数由宏定义：为“LCD_POWER_FULL”  “LCD_POWER_FULL_REDUCE”
*                             “LCD_POWER_FULL_MORE_REDUCE” “LCD_POWER_EMPTY”
*                             分别表示为“满电量” “减少一格电量” 
*                             “减少两格电量” “空电量”
* 备    注：在这里使用3、4、5号位显示数值5号位带小数点
************************************************************************************/
#if 0
void LCD_POWER(unsigned char PowerValue)
{
    TM1621C_init();		           //LCD初始化
    
    WriteAllData(0x19,0x10);               //电池空框架图
    switch(PowerValue)
    {
      case LCD_POWER_FULL:           
           WriteAllData(0x00,0x0F);        //电池容量满
        break;
      case LCD_POWER_FULL_REDUCE:
           WriteAllData(0x00,0x07);        //电池减少一格电量
        break;
      case LCD_POWER_FULL_MORE_REDUCE:
           WriteAllData(0x00,0x03);        //电池减少两格电量
        break;
      case LCD_POWER_FULL_MUCH_MORE_REDUCE:
           WriteAllData(0x00,0x01);        //电池减少三格电量
      default:
        break;
    }
}
#endif
/************************************************************************************
* 名    称：void LCD_POWER(unsigned char PowerValue)
* 功    能：电量显示
* 入口参数：需要显示的电量容量
* 出口参数：无
* 说    明：入口参数由宏定义：为“LCD_POWER_FULL”  “LCD_POWER_FULL_REDUCE”
*                             “LCD_POWER_FULL_MORE_REDUCE” “LCD_POWER_EMPTY”
*                             分别表示为“满电量” “减少一格电量” 
*                             “减少两格电量” “空电量”
*                             为“LCD_SIGNAL_FULL”  “LCD_SIGNAL_FULL_REDUCE”
*                             “LCD_SIGNAL_FULL_MORE_REDUCE” “LCD_SIGNAL_EMPTY”
*                             分别表示为“满信号” “减少一格信号” 
*                             “减少两格信号” “空信号”
* 备    注：在这里使用3、4、5号位显示数值5号位带小数点
************************************************************************************/
#if 0
void LCD_SIGNAL(unsigned char SignalValue)
{
    TM1621C_init();		           //LCD初始化
    
    WriteAllData(0x19,0x10);               //电池空框架图
    
    switch(SignalValue)
    {
      case LCD_SIGNAL_FULL:           
           WriteAllData(0x00,0xF0);        //信号满
        break;
      case LCD_SIGNAL_FULL_REDUCE:
           WriteAllData(0x00,0xE0);        //信号减少一格
        break;
      case LCD_SIGNAL_FULL_MORE_REDUCE:
           WriteAllData(0x00,0xC0);        //信号减少两格
        break;
      case LCD_SIGNAL_EMPTY:               //信号空
           WriteAllData(0x00,0x80);
        break;
      default:
        break;
    }
}
#endif

/************************************************************************************
* 名    称：void LCD_SIGNAL_POWER(unsigned char SignalValue, unsigned char PowerValue)
* 功    能：信号电量显示
* 入口参数：需要显示的信号和电量容量
* 出口参数：无
* 说    明：入口参数由宏定义：为“LCD_POWER_FULL”  “LCD_POWER_FULL_REDUCE”
*                             “LCD_POWER_FULL_MORE_REDUCE” “LCD_POWER_EMPTY”
*                             分别表示为“满电量” “减少一格电量” 
*                             “减少两格电量” “空电量”
* 备    注：在这里使用3、4、5号位显示数值5号位带小数点
************************************************************************************/
void LCD_SIGNAL_POWER(unsigned char SignalValue, unsigned char PowerValue)
{
    TM1621C_init();		           //LCD初始化
    
    WriteAllData(0x19,0x10);               //电池空框架图
    
    switch(SignalValue)
    {
        case LCD_SIGNAL_FULL:             //信号满
           switch(PowerValue)
           {
            case LCD_POWER_FULL:           
                 WriteAllData(0x00,LCD_SIGNAL_FULL_BIT | LCD_POWER_FULL_BIT); //电池容量满
                 return;
            case LCD_POWER_FULL_REDUCE:
                 WriteAllData(0x00,LCD_SIGNAL_FULL_BIT | LCD_POWER_FULL_REDUCE_BIT);        //电池减少一格电量
                 return;
            case LCD_POWER_FULL_MORE_REDUCE:
                 WriteAllData(0x00,LCD_SIGNAL_FULL_BIT | LCD_POWER_FULL_MORE_REDUCE_BIT);  //电池减少两格电量
                 return;
            case LCD_POWER_FULL_MUCH_MORE_REDUCE:
                 WriteAllData(0x00,LCD_SIGNAL_FULL_BIT | LCD_POWER_FULL_MUCH_MORE_REDUCE_BIT); //电池减少三格电量
                 return;
            default:
                 return;
           }
        break;
        case LCD_SIGNAL_FULL_REDUCE:                      //信号减少一格
           switch(PowerValue)
           {
            case LCD_POWER_FULL:           
                 WriteAllData(0x00,LCD_SIGNAL_FULL_REDUCE_BIT | LCD_POWER_FULL_BIT); //电池容量满
                 return;
            case LCD_POWER_FULL_REDUCE:
                 WriteAllData(0x00,LCD_SIGNAL_FULL_REDUCE_BIT | LCD_POWER_FULL_REDUCE_BIT);        //电池减少一格电量
                 return;
            case LCD_POWER_FULL_MORE_REDUCE:
                 WriteAllData(0x00,LCD_SIGNAL_FULL_REDUCE_BIT | LCD_POWER_FULL_MORE_REDUCE_BIT);  //电池减少两格电量
                 return;
            case LCD_POWER_FULL_MUCH_MORE_REDUCE:
                 WriteAllData(0x00,LCD_SIGNAL_FULL_REDUCE_BIT | LCD_POWER_FULL_MUCH_MORE_REDUCE_BIT); //电池减少三格电量
                 return;
            default:
                 return;
           }        
        break;
        case LCD_SIGNAL_FULL_MORE_REDUCE:                 //信号减少两格
           switch(PowerValue)
           {
            case LCD_POWER_FULL:           
                 WriteAllData(0x00,LCD_SIGNAL_FULL_MORE_REDUCE_BIT | LCD_POWER_FULL_BIT); //电池容量满
                 return;
            case LCD_POWER_FULL_REDUCE:
                 WriteAllData(0x00,LCD_SIGNAL_FULL_MORE_REDUCE_BIT | LCD_POWER_FULL_REDUCE_BIT);        //电池减少一格电量
                 return;
            case LCD_POWER_FULL_MORE_REDUCE:
                 WriteAllData(0x00,LCD_SIGNAL_FULL_MORE_REDUCE_BIT | LCD_POWER_FULL_MORE_REDUCE_BIT);  //电池减少两格电量
                 return;
            case LCD_POWER_FULL_MUCH_MORE_REDUCE:
                 WriteAllData(0x00,LCD_SIGNAL_FULL_MORE_REDUCE_BIT | LCD_POWER_FULL_MUCH_MORE_REDUCE_BIT); //电池减少三格电量
                 return;
            default:
                 return;
           }        
        break;
        case LCD_SIGNAL_EMPTY:                            //信号空
           switch(PowerValue)
           {
            case LCD_POWER_FULL:           
                 WriteAllData(0x00,LCD_SIGNAL_EMPTY_BIT | LCD_POWER_FULL_BIT); //电池容量满
                 return;
            case LCD_POWER_FULL_REDUCE:
                 WriteAllData(0x00,LCD_SIGNAL_EMPTY_BIT | LCD_POWER_FULL_REDUCE_BIT);        //电池减少一格电量
                 return;
            case LCD_POWER_FULL_MORE_REDUCE:
                 WriteAllData(0x00,LCD_SIGNAL_EMPTY_BIT | LCD_POWER_FULL_MORE_REDUCE_BIT);  //电池减少两格电量
                 return;
            case LCD_POWER_FULL_MUCH_MORE_REDUCE:
                 WriteAllData(0x00,LCD_SIGNAL_EMPTY_BIT | LCD_POWER_FULL_MUCH_MORE_REDUCE_BIT); //电池减少三格电量
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
* 名    称：void LcdNegNumber(short int Value)
* 功    能：正值显示
* 入口参数：需要显示的数值
* 出口参数：无
* 说    明：TM1621C LCD驱动控制器控制的这款LCD液晶有6个数字和5个小数点以及电量和
*           信号量 如下所示：
*                               8.  8.  8.  8.  8.  8           
*                               |   |   |   |   |   |           从左向右排1-6号数字8
*                              1号 2号 3号 4号 5号 6号
* 备    注：在这里使用3、4、5号位显示数值5号位带小数点
************************************************************************************/
void LcdPosNumber(int Value)
{
    unsigned int j;

    unsigned int dividedData[3];
    TM1621C_init();		//LCD初始化
                          
    dividedData[0]=Value/100;                   //百位
    dividedData[1]=Value%100/10;                //十位
    dividedData[2]=Value%10;                    //个位
    
    /*便于显示美观将数字都放置液晶屏的中心位置即数字从十位向左类推*/
    LCD_Number(WANWEI,  POSITIVE);               //正号 
    LCD_Number(SHIWEI,  dividedData[2]);         //十位
    LCD_Number(BAIWEI,  dividedData[1]);         //百位
    LCD_Number(QIANWEI, dividedData[0]);         //千位
    for (j =0;j<=800;j++);   
}

/************************************************************************************
* 名    称：void LcdNegNumber(short int Value)
* 功    能：负值显示
* 入口参数：需要显示的数值
* 出口参数：无
* 说    明：TM1621C LCD驱动控制器控制的这款LCD液晶有6个数字和5个小数点以及电量和
*           信号量 如下所示：
*                               8.  8.  8.  8.  8.  8           
*                               |   |   |   |   |   |           从左向右排1-6号数字8
*                              1号 2号 3号 4号 5号 6号
* 备    注：在这里使用2号位显示负号3、4、5号位显示数值5号位带小数点
************************************************************************************/
void LcdNegNumber(int Value)
{
    unsigned int j;

    unsigned int dividedData[3];
    TM1621C_init();		                //LCD初始化
                          
    dividedData[0]=Value/100;                   //百位
    dividedData[1]=Value%100/10;                //十位
    dividedData[2]=Value%10;                    //个位
    
    /*便于显示美观将数字都放置液晶屏的中心位置即数字从十位向左类推*/
    LCD_Number(WANWEI,  NEGATIVE);              //负号 
    LCD_Number(SHIWEI,  dividedData[2]);        //十位
    LCD_Number(BAIWEI,  dividedData[1]);        //百位
    LCD_Number(QIANWEI, dividedData[0]);        //千位
    for (j =0;j<=800;j++);   
}

/************************************************************************************
* 名    称：void LCD_Number(unsigned char position, unsigned char number)
* 功    能：显示数字量
* 入口参数1：position 由以下宏定义 “GEWEI”  “SHIWEI” “BAIWEI” “QIANWEI” “WANWEI”
* 入口参数2：number  所要显示的数字量
* 出口参数：无
* 备    注：1.在这里将万位的case 10作为符号位使用
*           2.在这里将十位带上小数点，为了当前设备能够保留一位小数
* 说    明：TM1621C LCD驱动控制器控制的这款LCD液晶有6个数字和5个小数点以及电量和
*           信号量 如下所示：
*                               8.  8.  8.  8.  8.  8           
*                               |   |   |   |   |   |           从左向右排1-6号数字8
*                              1号 2号 3号 4号 5号 6号
************************************************************************************/
void LCD_Number(unsigned char position, unsigned char number)
{  
  switch (position)//数字区域
  {
#if 0
    case GEWEI:	  
        switch(number)	  //个位 ->手册上6号位数字
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
         switch(number)   //十位 ->手册上5号位数字
        {
              case 0: WriteAllData(SHIWEI,0xBF);        //不带小数点将十六进制的高四位减一
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
        switch(number)	  //百位 ->手册上4号位数字
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
        switch(number)	 //千位 ->手册上3号位数字
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
         switch(number)   //万位 ->手册上2号位数字(注：十万位省略)
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
              WriteAllData(WANWEI,0x40);          //负号位
            break;
            case POSITIVE:
              WriteAllData(WANWEI,0x00);          //正号位
            break;
        }
        break;    
  }
}


/************************以下来自Battery和Rf************************/

/*
*       电池电量显示
*/
#if 0
void Power_Lcd_Display(void)
{
    float BatteryTemp = 0.0;
    
    BatteryTemp  = Battery_Value_Get();                 //电源模块电压
    
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
*       信号显示
*/
void Signal_Lcd_Display(void)
{
    int RfEnergyTemp = 0;

    
    CMD_MODE_ON;                //进入命令接收模式
    RfEnergyTemp = RF_RSSI_Get();
    TC_MODE_ON;                 //进入透传接收模式
    
    if(RfEnergyTemp > -40 && RfEnergyTemp <= 20)
       LCD_SIGNAL(LCD_SIGNAL_FULL);                     //信号满格
    else if(RfEnergyTemp > -64 && RfEnergyTemp <= -40)
       LCD_SIGNAL(LCD_SIGNAL_FULL_REDUCE);              //信号2格
    else if(RfEnergyTemp > -80 && RfEnergyTemp <= -64)
       LCD_SIGNAL(LCD_SIGNAL_FULL_MORE_REDUCE);         //信号1格
    else if(RfEnergyTemp > -87 && RfEnergyTemp <= -80)
       LCD_SIGNAL(LCD_SIGNAL_EMPTY);                    //无信号
  
}

#endif

/*
*       电量和信号监测显示
*/
void Symbol_Lcd_Display(void)
{  
    static u8  SignalFlag    = 0;
    static u8  PowerFlag     = 0;
    
/* 由于捕捉F8913信号存在模式切换导致应答不稳定影响程序，暂且假设信号满 */    
#if RSSI_TEST     //使用真信号
    static int RfEnergyTemp  = 0;
   
    
    CMD_MODE_ON;                //进入命令接收模式
    RfEnergyTemp = RF_RSSI_Get();
    TC_MODE_ON;                 //进入透传接收模式
    
    if(RfEnergyTemp > -40 && RfEnergyTemp <= 20)
       SignalFlag = LCD_SIGNAL_FULL;                     //信号满格
    else if(RfEnergyTemp > -64 && RfEnergyTemp <= -40)
       SignalFlag = LCD_SIGNAL_FULL_REDUCE;              //信号2格
    else if(RfEnergyTemp > -80 && RfEnergyTemp <= -64)
       SignalFlag = LCD_SIGNAL_FULL_MORE_REDUCE;         //信号1格
    else if(RfEnergyTemp > -87 && RfEnergyTemp <= -80)
       SignalFlag = LCD_SIGNAL_EMPTY;                    //无信号
    else if(RfEnergyTemp == 100)
       SignalFlag = LCD_SIGNAL_EMPTY;                    //由于返回100错误信息。选择空信号作为输入
#else   //使用假信号
    SignalFlag = LCD_SIGNAL_FULL;
#endif

#if BATTERY_TEST
    static float BatteryTemp = 0.0;
    
    BatteryTemp  = Battery_Value_Get();                 //电源模块电压
    
    if( BatteryTemp > 3.2 )
      PowerFlag = LCD_POWER_FULL;                               //满格
    else if( BatteryTemp > 3.1 && BatteryTemp <= 3.2 )
      PowerFlag = LCD_POWER_FULL_REDUCE;                        //3格
    else if( BatteryTemp > 2.8 && BatteryTemp <= 3.1 )
      PowerFlag = LCD_POWER_FULL_MORE_REDUCE;                   //2格
    else if( BatteryTemp <= 2.8 )
      PowerFlag = LCD_POWER_FULL_MUCH_MORE_REDUCE;              //1格
#else
    PowerFlag = LCD_POWER_FULL;
#endif    
    LCD_SIGNAL_POWER(SignalFlag, PowerFlag);
}