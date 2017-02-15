#ifndef __RF_H
#define __RF_H

#include "includes.h"


#define RADIO_RST_LOW       P4DIR |= BIT2;P4OUT &= ~BIT2   //F8913DÐ¾Æ¬¸´Î»
#define RADIO_RST_HIGH      P4DIR |= BIT2;P4OUT |= BIT2


#define F8913D_SLEEP_RQ     P4DIR|=BIT7;P4OUT |= BIT7      //Éî¶ÈË¯Ãß
#define F8913D_N_SLEEP_RQ   P4DIR|=BIT7;P4OUT&=~BIT7       //F8913D»½ÐÑ
#define ASSOCIATE_STATE     (P4IN & BIT6)                  //ÍøÂç×´Ì¬


/* ----------------------------------------------------------------- */
/*                        RF COMMAND DEFINATION                      */
/* ----------------------------------------------------------------- */
#define RSP	"\r\nOK\r\n"
#define FINISH "\r\n"
#define AT_MODE	"1"

#define AT      "AT\r\n"
#define QMA     "AT+QMA=0\r\n"
#define ESC     "AT+ESC\r\n"



void RF_Send_Data(char *strptr, u32 len);
int  RF_CMD(char *cmd, char *response, u8 timeout);
int  RF_RSSI_Get(void);
#endif
