#ifndef __LCD_TM1621_H
#define __LCD_TM1621_H

#include <includes.h>

/********************������ƶ˿�**********************/
#define LCD_CS_SET       P4OUT |= BIT3
#define LCD_CS_CLR       P4OUT &=~ BIT3

#define LCD_WRITE_SET    P4OUT |= BIT4
#define LCD_WRITE_CLR    P4OUT &=~ BIT4

#define LCD_DATA_SET     P4OUT |= BIT5
#define LCD_DATA_CLR     P4OUT &=~ BIT5

#define nop _NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP(); //�궨��


/******************TM1621Cģ�������*********************/
#define SYSDIS   0x00	  //��ϵͳ������LCDƫѹ������	
#define SYSEN    0x02     //��ϵͳ����	
#define LCDOFF   0x04     //��LCDƫѹ
#define LCDON    0x06     //��LCDƫѹ							 
#define RC       0x30     //�ڲ�RC��	
#define BIAS     0x52     //1/3ƫѹ 4������	

#define GEWEI    0x02     //��λ��ַ
#define SHIWEI   0x04     //ʮλ��ַ
#define BAIWEI   0x0D     //��λ��ַ
#define QIANWEI  0x0F     //ǧλ��ַ
#define WANWEI   0x11     //��λ��ַ

#define NEGATIVE 10       //������
#define POSITIVE 11       //������

/* �����궨�� */
#define LCD_POWER_EMPTY                 0       //�յ���
#define LCD_POWER_FULL                  1       //������
#define LCD_POWER_FULL_REDUCE           2       //����һ�����
#define LCD_POWER_FULL_MORE_REDUCE      3       //�����������
#define LCD_POWER_FULL_MUCH_MORE_REDUCE 4       //�����������
/* �źź궨�� */
#define LCD_SIGNAL_FULL                 1       //���ź�
#define LCD_SIGNAL_FULL_REDUCE          2       //����һ���ź�
#define LCD_SIGNAL_FULL_MORE_REDUCE     3       //���������ź�
#define LCD_SIGNAL_EMPTY                4       //���ź�


#define LCD_POWER_FULL_BIT                  0x0F       //������
#define LCD_POWER_FULL_REDUCE_BIT           0x07       //����һ�����
#define LCD_POWER_FULL_MORE_REDUCE_BIT      0x03       //�����������
#define LCD_POWER_FULL_MUCH_MORE_REDUCE_BIT 0x01       //�����������
#define LCD_SIGNAL_FULL_BIT                 0xF0       //���ź�
#define LCD_SIGNAL_FULL_REDUCE_BIT          0xE0       //����һ���ź�
#define LCD_SIGNAL_FULL_MORE_REDUCE_BIT     0xC0       //���������ź�
#define LCD_SIGNAL_EMPTY_BIT                0x80       //���ź�



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