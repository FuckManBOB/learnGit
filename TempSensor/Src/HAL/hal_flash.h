#ifndef __HAL_FLASH_
#define __HAL_FLASH_

#include "includes.h"

#define FLASH_SEG_A             1
#define FLASH_SEG_B             2
#define FLASH_SEGA_ADDR         (char *) 0x1080
#define FLASH_SEGB_ADDR         (char *) 0x1000

char FlashSegErase(unsigned char FlashSeg);
char FlashWrite(unsigned char FlashSeg, char *Dataptr, unsigned char DataLength);


#endif