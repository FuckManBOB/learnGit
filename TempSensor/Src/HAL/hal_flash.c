#include "hal_flash.h"


/****************************************************************************
* 名    称：unsigned char FlashSegErase(unsigned char FlashSeg)
* 功    能：擦除flash段
* 入口参数：段A或段B 入参值为1或者2
* 出口参数：0-写入段有误 1-擦除成功
* 备    注：段A和段B分别为128字节，段A起始地址为0x1080 段B起始地址为0x1000
****************************************************************************/
char FlashSegErase(unsigned char FlashSeg)
{
  char *Flash_ptr;
  switch(FlashSeg)
  {
    case FLASH_SEG_A:
        Flash_ptr = FLASH_SEGA_ADDR;       
        break;
    case FLASH_SEG_B:
        Flash_ptr = FLASH_SEGB_ADDR;
        break;
  default:
        return FAIL;
  }
  FCTL1 = FWKEY + ERASE;                  // Set Erase bit
  FCTL3 = FWKEY;                          // Clear Lock bit
  *Flash_ptr = 0;
  FCTL3 = FWKEY + LOCK;                     // Set LOCK bit
  return SECCESS;
}


/****************************************************************************
* 名    称：void FlashWrite(unsigned char FlashSeg,char *dataptr)
* 功    能：flash段A或B中写入数据
* 入口参数1：段A或段B 入参值为1或者2，
* 入口参数2：写入内容
* 出口参数：0-写入段有误或数据长度超限   1-写入成功
* 备    注：段A和段B分别为128字节，段A起始地址为0x1080 段B起始地址为0x1000（无需擦除函数中带擦除代码）
****************************************************************************/
char FlashWrite(unsigned char FlashSeg, char *Dataptr, unsigned char DataLength)
{
  char *Flash_ptr;                          // Flash pointer
  unsigned int i;
  
  if(DataLength > 128)
    return FAIL;
  switch(FlashSeg)
  {
    case FLASH_SEG_A:
        Flash_ptr = FLASH_SEGA_ADDR;              // Initialize Flash pointer
        break;
    case FLASH_SEG_B:
        Flash_ptr = FLASH_SEGB_ADDR;              // Initialize Flash pointer
        break;
    default:
        return FAIL;    
  }
  FCTL1 = FWKEY + ERASE;                    // Set Erase bit
  FCTL3 = FWKEY;                            // Clear Lock bit
  *Flash_ptr = 0;                           // Dummy write to erase Flash segment

  FCTL1 = FWKEY + WRT;                      // Set WRT bit for write operation

  for (i = 0; i < DataLength; i++)
  {
    *Flash_ptr++ = *Dataptr++;              // Write data to flash
  }
  FCTL1 = FWKEY;                            // Clear WRT bit
  FCTL3 = FWKEY + LOCK;                     // Set LOCK bit
  return SECCESS;
}

