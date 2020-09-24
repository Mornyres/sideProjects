/* Includes ------------------------------------------------------------------*/
#include <myFat_diskio.h>



#if defined ( __GNUC__ )
#ifndef __weak
#define __weak __attribute__((weak))
#endif
#endif

static volatile DSTATUS Stat = STA_NOINIT;


/**
  * @brief  Initializes a Drive
  * @param  pdrv: Physical drive number (0..)
  * @retval DSTATUS: Operation status
  */
DSTATUS WYT_disk_initialize (
		void
)
{
	return WYT_disk_status();
}

DSTATUS WYT_disk_status(void)
{
  Stat = STA_NOINIT;

  if(BSP_SD_GetCardState() == MSD_OK)
  {
    Stat &= ~STA_NOINIT;
  }

  return Stat;
}

/**
  * @brief  Reads Sector(s)
  * @param  pdrv: Physical drive number (0..)
  * @param  *buff: Data buffer to store read data
  * @param  sector: Sector address (LBA)
  * @param  count: Number of sectors to read (1..128)
  * @retval DRESULT: Operation result
  */
DRESULT WYT_disk_read (
	BYTE *buff,		/* Data buffer to store read data */
	DWORD sector,	        /* Sector address in LBA */
	UINT offset,	/* Offset in the sector */
	UINT count		/* Number of sectors to read */
)
{
	  DRESULT res = ERROR_CODE;
	  uint8_t BSPresult = BSP_SD_ReadBlocks((uint32_t*)buff, (uint32_t) (sector), count, SD_DATATIMEOUT);
	  if(BSPresult == MSD_OK)
	  {
	    /* wait until the read operation is finished */
	    while(BSP_SD_GetCardState()!= MSD_OK)
	    {
	    }
	    res = OK;
	  }

	  else {
		msglen = snprintf(msgBuf, 255, "ERR: BSP layer failed with BSP_SD_ReadBlocks = %d\r\n", BSPresult);
		debugMessage(msgBuf,msglen);

		_Error_Handler(__FILE__, __LINE__);

	  }

	  return res;
}


DRESULT WYT_disk_write (
	const BYTE *buff,	/* Data to be written */
	DWORD sector,		/* Sector address in LBA */
	UINT count        	/* Number of sectors to write */
)
{
	  DRESULT res = ERROR_CODE;

	  if(BSP_SD_WriteBlocks((uint32_t*)buff,
	                        (uint32_t)(sector),
	                        count, SD_DATATIMEOUT) == MSD_OK)
	  {
		/* wait until the Write operation is finished */
	    while(BSP_SD_GetCardState() != MSD_OK)
	    {
	    }
	    res = OK;
	  }

	  return res;
}


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

