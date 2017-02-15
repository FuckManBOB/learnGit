#ifndef	__LCD_H
#define __LCD_H

#include "includes.h"

#define u16     unsigned short int
#define u8      unsigned char

//====< LCD IO define area >=======================================	  
#define CS_LOW    P4OUT &= ~BIT3  
#define CS_HIGH   P4OUT |= BIT3
//#define RD        (P4IN & BIT5)       //没用上
#define WR_LOW    P4OUT &= ~BIT4
#define WR_HIGH   P4OUT |= BIT4

#define DATA_LOW  P4OUT &= ~BIT5
#define DATA_HIGH P4OUT |= BIT5


//====< LCD command define area >=======================================
#define SYS_EN          	0x01
#define SYS_DIS         	0x00
#define LCD_ON          	0x03
#define LCD_OFF         	0x02
#define	RC_Clock		0x18       //HT1621:RC256K  HT1622 RC32K
#define	LCD_1_3BIAS1_4DUTY	0x29
#define	LCD_1_2BIAS1_4DUTY	0x28
#define	LCD_1_3BIAS1_3DUTY	0x25
#define	LCD_1_2BIAS1_3DUTY	0x24
#define	LCD_1_2BIAS1_2DUTY	0x20


void SendLcdCmd(u8 cmd);
//void SendLcdData(u8 addr,u8 data);

void LcdIo_Init(void);
void LCD_Init(void);

void Test_LCD_Reset(void);
void Write_3208_Data(u8 nibble,u8 b,u8 addr);


void power_state(u8 state);
void signal_state(u8 state);
void lcd_number(u8 position, u8 number);
void Lcd_Signal_data(void);
void Lcd_Number_data(void);

void LcdClr_All(void);
void LcdClr_Number(void);
void LcdClr_Signal(void);


void LcdPosNumber(short int Temperature);
void LcdNegNumber(short int Temperature);
void LcdPowerSignalState(u8 powerState,u8 signalState);

void LcdClean(void);

void Symbol_Lcd_Display(void);//电池电量监测显示

#endif
