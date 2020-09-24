/*	@file		: config.h */
#ifndef __CONFIG_H
#define __CONFIG_H
/*
 * 		USART1	--	RFID reader
 * 					9600 8N1
 * 		USART2	--	Serial debugging
 * 					115200 8N1
 * 		USART3	--	Xbee
 * 					9600 8N1
 * 					PAN 3302
 * 					Transparent mode
 *
 * 		I2C1	--	IMU
 * 		I2C2	--	[reserved]
 * 		I2C3	--	[reserved]
 *
 * 		SDMMC1	--	SD card
 * 					4B
 *
 * 		Pseudocode:
 * 			Request tag
 * 			Time to upload?
 * 				Yes:
 * 					Upload last X readings from SD card
 * 				No:
 * 					Store on SD
 *
 */

#include "stm32l4xx_hal.h"
#include "stm32l4xx_hal_usart.h"
//#include <stdio.h>
#include <string.h>

// Settings for application performance
#define SDMMC_ENABLE 0
#define SPI_ENABLE 1
#define VERBOSE 1
#define SDMMC_WIDTH SDMMC_BUS_WIDE_1B

// strictly for debugging purposes:
#define IGNORE_ERRORS 1

// Pin definitions
#define card_detect_Pin GPIO_PIN_0
#define card_detect_GPIO_Port GPIOB


#define GREENLED_Pin GPIO_PIN_10
#define GREENLED_Port GPIOA

// red led redemption
#define REDLED_Pin GPIO_PIN_9
#define REDLED_Port GPIOA

#define YELLOWLED_Pin GPIO_PIN_8
#define YELLOWLED_Port GPIOA

#define usart1_reader_TX_Pin GPIO_PIN_9
#define usart1_reader_TX_GPIO_Port GPIOA
#define usart1_reader_RX_Pin GPIO_PIN_10
#define usart1_reader_RX_GPIO_Port GPIOA
#define usart1_reader_Pin GPIO_PIN 8
#define usart1_reader_GPIO_Port GPIOA

#define usart2_debugTX_Pin GPIO_PIN_2
#define usart2_debugTX_GPIO_Port GPIOA
#define usart2_debugRX_Pin GPIO_PIN_3
#define usart2_debugRX_GPIO_Port GPIOA
#define usart2_debugCLK_Pin GPIO_PIN 4
#define usart2_debugRX_GPIO_Port GPIOA

#define SDMMC_CMD_Pin GPIO_PIN_2
#define SDMMC_CMD_GPIO_Port GPIOD
#define SDMMC_D2_Pin GPIO_PIN_10
#define SDMMC_D2_GPIO_Port GPIOC
#define SDMMC_D3_Pin GPIO_PIN_11
#define SDMMC_D3_GPIO_Port GPIOC
#define SDMMC_CLK_Pin GPIO_PIN_12
#define SDMMC_CLK_GPIO_Port GPIOC
#define SDMMC_D1_Pin GPIO_PIN_9
#define SDMMC_D1_GPIO_Port GPIOC
#define SDMMC_D0_Pin GPIO_PIN_8
#define SDMMC_D0_GPIO_Port GPIOC

#define SPI_CLK_Pin GPIO_PIN 5
#define SPI_CLK_GPIO_Port GPIOA
#define SPI_MISO_Pin GPIO_PIN 6
#define SPI_MISO_GPIO_Port GPIOA
#define SPI_MOSI_Pin GPIO_PIN 7
#define SPI_MOSI_GPIO_Port GPIOA

#define SPIx                                 SPI1
#define SPIx_CLK_ENABLE()                    __HAL_RCC_SPI1_CLK_ENABLE()

#define SPIx_SCK_AF                          GPIO_AF5_SPI1
#define SPIx_SCK_GPIO_PORT                   GPIOA
#define SPIx_SCK_PIN                         GPIO_PIN_5
#define SPIx_SCK_GPIO_CLK_ENABLE()           __HAL_RCC_GPIOA_CLK_ENABLE()
#define SPIx_SCK_GPIO_CLK_DISABLE()          __HAL_RCC_GPIOA_CLK_DISABLE()

#define SPIx_MISO_MOSI_AF                    GPIO_AF5_SPI1
#define SPIx_MISO_MOSI_GPIO_PORT             GPIOA
#define SPIx_MISO_MOSI_GPIO_CLK_ENABLE()     __HAL_RCC_GPIOA_CLK_ENABLE()
#define SPIx_MISO_MOSI_GPIO_CLK_DISABLE()    __HAL_RCC_GPIOA_CLK_DISABLE()
#define SPIx_MISO_PIN                        GPIO_PIN_6
#define SPIx_MOSI_PIN                        GPIO_PIN_7

#define SD_CARD_NOT_FORMATTED                    	0
#define SD_CARD_FILE_NOT_SUPPORTED               	1
#define SD_CARD_OPEN_FAIL                        	2
#define FATFS_NOT_MOUNTED                        	3
#define BSP_SD_INIT_FAILED                       	4

#define MODE_TAG  0x01
#define MODE_RANDOM  0x02

static const char *Lookup_BSP[] = {
		"MSD_OK\0",						// 0
		"BSP_SD_ERROR\0",					// 1
		// couldn't config for desired data width
		"BSP_SD_ERROR_SD_NOT_PRESENT\0"	// 2
};

static const char *Lookup_fresult[] = {
		"FR_OK\0"						// 0
		,"FR_DISK_ERR\0"				// 1
		,"FR_INT_ERR\0"					// 2
		,"FR_NOT_READY\0"				// 3
		,"FR_NO_FILE\0"					// 4
		,"FR_NO_PATH\0"					// 5
		,"FR_INVALID_NAME\0"			// 6
		,"FR_DENIED\0"					// 7
		,"FR_EXIST\0"					// 8
		,"FR_INVALID_OBJECT\0"			// 9
		,"FR_WRITE_PROTECTED\0"			// 10
		,"FR_INVALID_DRIVE\0"			// 11
		,"FR_NOT_ENABLED\0"				// 12
		,"FR_NO_FILESYSTEM\0"			// 13
		,"FR_MKFS_ABORTED\0"			// 14
		,"FR_TIMEOUT\0"					// 15
		,"FR_LOCKED\0"					// 16
		,"FR_NOT_ENOUGH_CORE\0"			// 17
		,"FR_TOO_MANY_OPEN_FILES\0"		// 18
		,"FR_INVALID_PARAMETER\0"		// 19
};

static const char *Lookup_HAL[] = {
		"HAL_OK\0"
		,"HAL_ERROR\0"
		,"HAL_BUSY\0"
		,"HAL_TIMEOUT\0"
};

static const char *Lookup_HALSD[] = {
		"HAL_SD_ERROR_NONE\0"						// 0
		,"HAL_SD_ERROR_CMD_CRC_FAIL\0"            // 1
		,"HAL_SD_ERROR_DATA_CRC_FAIL\0"           // 2
		,"HAL_SD_ERROR_CMD_RSP_TIMEOUT\0"         // 3
		,"HAL_SD_ERROR_DATA_TIMEOUT\0"            // 4
		,"HAL_SD_ERROR_TX_UNDERRUN\0"             // 5
		,"HAL_SD_ERROR_RX_OVERRUN\0"              // 6
		,"HAL_SD_ERROR_ADDR_MISALIGNED\0"         // 7
		,"HAL_SD_ERROR_BLOCK_LEN_ERR\0"           // 8
		,"HAL_SD_ERROR_ERASE_SEQ_ERR\0"           // 9
		,"HAL_SD_ERROR_BAD_ERASE_PARAM\0"         // 10
		,"HAL_SD_ERROR_WRITE_PROT_VIOLATION\0"    // 11
		,"HAL_SD_ERROR_LOCK_UNLOCK_FAILED\0"      // 12
		,"HAL_SD_ERROR_COM_CRC_FAILED\0"          // 13
		,"HAL_SD_ERROR_ILLEGAL_CMD\0"             // 14
		,"HAL_SD_ERROR_CARD_ECC_FAILED\0"         // 15
		,"HAL_SD_ERROR_CC_ERR\0"                  // 16
		,"HAL_SD_ERROR_GENERAL_UNKNOWN_ERR\0"     // 17
		,"HAL_SD_ERROR_STREAM_READ_UNDERRUN\0"    // 18
		,"HAL_SD_ERROR_STREAM_WRITE_OVERRUN\0"    // 19
		,"HAL_SD_ERROR_CID_CSD_OVERWRITE\0"       // 20
		,"HAL_SD_ERROR_WP_ERASE_SKIP\0"           // 21
		,"HAL_SD_ERROR_CARD_ECC_DISABLED\0"       // 22
		,"HAL_SD_ERROR_ERASE_RESET\0"             // 23
		,"HAL_SD_ERROR_AKE_SEQ_ERR\0"             	// 24
		,"HAL_SD_ERROR_INVALID_VOLTRANGE\0"       	// 25
		,"HAL_SD_ERROR_ADDR_OUT_OF_RANGE\0"       	// 26
		,"HAL_SD_ERROR_REQUEST_NOT_APPLICABLE\0"	// 27
		,"HAL_SD_ERROR_PARAM\0"                   // 28
		,"HAL_SD_ERROR_UNSUPPORTED_FEATURE\0"     // 29
		,"HAL_SD_ERROR_BUSY\0"                    // 30
		,"HAL_SD_ERROR_DMA\0"                     // 31
		,"HAL_SD_ERROR_TIMEOUT\0"                 // 32
};

extern USART_HandleTypeDef husart_debug;

extern int msglen;
extern char msgBuf[255];

extern int transmitLen;
extern char transmitBuf[255];

int debugMessage(char* msgBuf, int len);

/* USART2 init function */
void DEBUG_USART_Init(void);

void _Error_Handler(char *file, int line);

#endif /* __CONFIG_H_ */
