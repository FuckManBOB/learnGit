#include "hal_flash.h"


/****************************************************************************
* ��    �ƣ�unsigned char FlashSegErase(unsigned char FlashSeg)
* ��    �ܣ�����flash��
* ��ڲ�������A���B ���ֵΪ1����2
* ���ڲ�����0-д������� 1-�����ɹ�
* ��    ע����A�Ͷ�B�ֱ�Ϊ128�ֽڣ���A��ʼ��ַΪ0x1080 ��B��ʼ��ַΪ0x1000
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
* ��    �ƣ�void FlashWrite(unsigned char FlashSeg,char *dataptr)
* ��    �ܣ�flash��A��B��д������
* ��ڲ���1����A���B ���ֵΪ1����2��
* ��ڲ���2��д������
* ���ڲ�����0-д�����������ݳ��ȳ���   1-д��ɹ�
* ��    ע����A�Ͷ�B�ֱ�Ϊ128�ֽڣ���A��ʼ��ַΪ0x1080 ��B��ʼ��ַΪ0x1000��������������д��������룩
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

