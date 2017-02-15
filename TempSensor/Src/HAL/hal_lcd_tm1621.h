#ifndef __LCD_TM1621_H
#define __LCD_TM1621_H

#include <includes.h>

/********************定义控制端口**********************/
#define LCD_CS_SET       P4OUT |= BIT3
#define LCD_CS_CLR       P4OUT &=~ BIT3

#define LCD_WRITE_SET    P4OUT |= BIT4
#define LCD_WRITE_CLR    P4OUT &=~ BIT4

#define LCD_DATA_SET     P4OUT |= BIT5
#define LCD_DATA_CLR     P4OUT &=~ BIT5

#define nop _NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP(); //宏定义


/******************TM1621C模块命令定义*********************/
#define SYSDIS   0x00	  //关系统振荡器和LCD偏压发生器	
#define SYSEN    0x02     //打开系统振荡器	
#define LCDOFF   0x04     //关LCD偏压
#define LCDON    0x06     //开LCD偏压							 
#define RC       0x30     //内部RC振荡	
#define BIAS     0x52     //1/3偏压 4公共口	

#define GEWEI    0x02     //个位地址
#define SHIWEI   0x04     //十位地址
#define BAIWEI   0x0D     //百位地址
#define QIANWEI  0x0F     //千位地址
#define WANWEI   0x11     //万位地址

#define NEGATIVE 10       //负符号
#define POSITIVE 11       //正符号

/* 电量宏定义 */
#define LCD_POWER_EMPTY                 0       //空电量
#define LCD_POWER_FULL                  1       //满电量
#define LCD_POWER_FULL_REDUCE           2       //减少一格电量
#define LCD_POWER_FULL_MORE_REDUCE      3       //减少两格电量
#define LCD_POWER_FULL_MUCH_MORE_REDUCE 4       //减少三格电量
/* 信号宏定义 */
#define LCD_SIGNAL_FULL                 1       //满信号
#define LCD_SIGNAL_FULL_REDUCE          2       //减少一格信号
#define LCD_SIGNAL_FULL_MORE_REDUCE     3       //减少两格信号
#define LCD_SIGNAL_EMPTY                4       //空信号


#define LCD_POWER_FULL_BIT                  0x0F       //满电量
#define LCD_POWER_FULL_REDUCE_BIT           0x07       //减少一格电量
#define LCD_POWER_FULL_MORE_REDUCE_BIT      0x03       //减少两格电量
#define LCD_POWER_FULL_MUCH_MORE_REDUCE_BIT 0x01       //减少三格电量
#define LCD_SIGNAL_FULL_BIT                 0xF0       //满信号
#define LCD_SIGNAL_FULL_REDUCE_BIT          0xE0       //减少一格信号
#define LCD_SIGNAL_FULL_MORE_REDUCE_BIT     0xC0       //减少两格信号
#define LCD_SIGNAL_EMPTY_BIT                0x80       //空信号



void TM1621C_init();
void LcdClean(void);
void LcdPosNumber(int Value);
void LcdNegNumber(int Value);
void LCD_Number(unsigned char position, unsigned char number);
void LCD_POWER(unsigned char PowerValue);
void LCD_SIGNAL(unsigned char SignalValue);
void Power_Lcd_Display(void);
void Signal_Lcd_Display(void);
void Symbol_Lcd_Display(void);
void LCD_SIGNAL_POWER(unsigned char SignalValue, unsigned char PowerValue);
void WriteAllData(char Addr,char p);
#endif