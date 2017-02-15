#ifndef __HAL_BOARD_H
#define __HAL_BOARD_H

#include "includes.h"

#define RF_UTXEN        IE2 |= UTXIE1
#define RF_UTXDIS       IE2 &=~UTXIE1

#if LED_DEBUG
#define LED_OFF          P1OUT |= BIT4
#define LED_ON           P1OUT &=~BIT4
#endif


void BSP_Init(void);
void Uart_Rf_SendByte(char data);
void Uart0_SendStr(char *strptr, u8 len);
void OS_TickStart (void);
#endif