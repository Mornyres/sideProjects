/*
 * 2/12 update:
 * 1. Finish FAT library (BSP instrumentation)
 * 2. Serial debug (redirect to SWDIO)
 * 3. Accept generic serial input
 *
 */

#include "main.h"
#include "global_config.h"
#include "rtc_config.h"

FATFS SD_FatFs;  /* File system object for SD card logical drive */
char SD_Path[4]; /* SD card logical drive path */
FRESULT fres;

FIL _StatusFile;
FIL _DataFile;

SPI_HandleTypeDef hspi1;
DMA_HandleTypeDef hdma_spi1_rx;
DMA_HandleTypeDef hdma_spi1_tx;

USART_HandleTypeDef husart_data1;
USART_HandleTypeDef husart_debug;


/* global time and date structs to be manipulated by subsequent calls to syncTime() */
RTC_TimeTypeDef sTime;
RTC_DateTypeDef sDate;
RTC_HandleTypeDef hrtc;

#define MAX_ITERATIONS 5

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void ff_SDConfig(void);

static void MX_GPIO_Init(void);
static void MX_RTC_Init(void);

// size and data are inputs
static int ff_logData(const char* data, uint32_t size);
static int myFat_test(void);
static int RTC_TimeStamp(char* stampDate, char* stampTime);
static int dummyData(char* data, uint16_t size, uint8_t mode);

int debugMessage(char* msgBuf, int len);

int requestData(USART_HandleTypeDef dataUSART);

static int syncTime(void);

int msglen = 0;
char msgBuf[255] = "\0";

USART_HandleTypeDef husart_debug;

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
 int main(void)
{
	int iteration = 0;
	// these are to be copied to, not from
	char currentTime[255] = "0";
	char currentDate[255] = "0";

	HAL_Init();

	/* Configure the system clock = 80 MHz */
	SystemClock_Config();

	MX_GPIO_Init();
	MX_RTC_Init();
	DEBUG_USART_Init();

#if defined(__TIME__) && defined( __DATE__)
	msglen = snprintf(msgBuf, 255, "Preprocessor Defined Build Time: %s\r\nDate: %s\r\n\r\n", __TIME__, __DATE__);
	debugMessage(msgBuf,msglen);
#endif

	RTC_TimeStamp(currentDate,currentTime);

	msglen = snprintf(msgBuf, 255, "RTC reported date & time (YY-MM-DD|HH:MM:SS) -- %s|%s\r\n",currentDate,currentTime);
	debugMessage(msgBuf,msglen);

	msglen = snprintf(msgBuf, 255, "STAT: Peripherals initialized\r\n");
	debugMessage(msgBuf,msglen);

#if VERBOSE == 1
	msglen = snprintf(msgBuf, 255, "STAT: Sample random tag: ");
	debugMessage(msgBuf,msglen);
	dummyData(msgBuf, 24, MODE_TAG);

	msglen = snprintf(msgBuf, 255, "[%s]\r\n",msgBuf);
	debugMessage(msgBuf,msglen);
#endif

	msglen = snprintf(msgBuf, 255, "STAT: Attempting to initialize myFat...\r\n");
	debugMessage(msgBuf,msglen);

	//if(myFat_test() != OK){
	if (1){

		msglen = snprintf(msgBuf, 255, "STAT: Failed to initialize myFat. Switching to fatfs for reference... \r\n");
		debugMessage(msgBuf,msglen);

		/* Configure SD card */
		ff_SDConfig();
		while (iteration < MAX_ITERATIONS){
			// write some data with timestamp, delay 5ish seconds, go again
			HAL_Delay(1 * 1000);

			// place a sample tag in buffer for writing
			dummyData(msgBuf, 24, MODE_TAG);

			msglen = snprintf(msgBuf, 255, "%s \r\n", msgBuf);

			ff_logData(msgBuf, msglen);
			iteration++;

		}
	}

	msglen = snprintf(msgBuf, 255, "STAT: Entering main loop\r\n");
	debugMessage(msgBuf,msglen);

	/* Infinite loop */


	while (1)
	{
	}
}


void SystemClock_Config(void)
{

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_PeriphCLKInitTypeDef PeriphClkInit;

    /**Configure LSE Drive Capability
    */
  HAL_PWR_EnableBkUpAccess();

  __HAL_RCC_LSEDRIVE_CONFIG(RCC_LSEDRIVE_LOW);

    /**Initializes the CPU, AHB and APB buses' clocks
    */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSE|RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.LSEState = RCC_LSE_ON;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSICalibrationValue = RCC_MSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_6;

  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_MSI;
  RCC_OscInitStruct.PLL.PLLM = 1;
  RCC_OscInitStruct.PLL.PLLN = 40;
  RCC_OscInitStruct.PLL.PLLR = 2;
  RCC_OscInitStruct.PLL.PLLP = 7;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if(HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    /* Initialization Error */
    while(1);
  }
  //

  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Initializes the CPU, AHB and APB busses clocks
    */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_MSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }



  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_RTC
		  	  	  	  	  	  	  	  |RCC_PERIPHCLK_USART1
									  |RCC_PERIPHCLK_USART2
									  ;
  PeriphClkInit.RTCClockSelection = RCC_RTCCLKSOURCE_LSE;
  PeriphClkInit.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK2;
  PeriphClkInit.Usart2ClockSelection = RCC_USART2CLKSOURCE_PCLK1;

  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Configure the main internal regulator output voltage
    */
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Configure the Systick interrupt time
    */
  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

    /**Configure the Systick
    */
  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

    /**Enable MSI Auto calibration
    */
  HAL_RCCEx_EnableMSIPLLMode();

  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

static void MX_GPIO_Init(void)
{

  GPIO_InitTypeDef GPIO_InitStruct;

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  //__HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /*
  // Configure GPIO pin Output Level
  HAL_GPIO_WritePin(CARD_DETECT_GPIO_Port, CARD_DETECT_Pin, GPIO_PIN_RESET);

  Configure GPIO pin : CARD_DETECT_Pin
  GPIO_InitStruct.Pin = CARD_DETECT_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(CARD_DETECT_GPIO_Port, &GPIO_InitStruct);
  */

}

static int syncTime(void){


	// HH ... -48 ('0') to go from ascii code to number
	// separately get tens and ones place
	sTime.Hours =
	((__TIME__[0] - '0') * 10) +
	(__TIME__[1] - '0');

	// MM
	sTime.Minutes =
	((__TIME__[3] - '0') * 10) +
	(__TIME__[4] - '0');

	// SS
	sTime.Seconds =
	((__TIME__[6] - '0') * 10) +
	(__TIME__[7] - '0');

	// todo: special conversion stuff for date because null/space chars may exist
	// __DATE__ PPD used

	sDate.Year = BUILD_YEAR;

	sDate.Month = BUILD_MONTH;
	sDate.Date=BUILD_DAY;


	return 0;

}

/* RTC init function */
static void MX_RTC_Init(void)
{

    /**Initialize RTC Only
    */
  hrtc.Instance = RTC;
  hrtc.Init.HourFormat = RTC_HOURFORMAT_24;
  hrtc.Init.AsynchPrediv = 127;
  hrtc.Init.SynchPrediv = 255;
  hrtc.Init.OutPut = RTC_OUTPUT_DISABLE;
  hrtc.Init.OutPutRemap = RTC_OUTPUT_REMAP_NONE;
  hrtc.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
  hrtc.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;
  if (HAL_RTC_Init(&hrtc) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Initialize RTC and set the Time and Date
    */

  syncTime();

  sTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
  sTime.StoreOperation = RTC_STOREOPERATION_RESET;
  if (HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BCD) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

	if (HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BCD) != HAL_OK)
	{
		_Error_Handler(__FILE__, __LINE__);
	}

}


// basically an abstraction of write function for the data file
// todo: APPEND, do not overwrite
static int ff_logData(const char* data, uint32_t size)
{
	char currentTime[255] = "0";
	char currentDate[255] = "0";

	int logLen = 255;
	char logBuf[255] = "0";

	int retval = -1;

	/* Check the mounted device */
	if(f_mount(&SD_FatFs, (TCHAR const*)"/", 1) != FR_OK)
	{
		_Error_Handler(__FILE__, __LINE__);
	}
	else
	{
		  fres = f_open(&_DataFile, "ff_Data.txt", FA_OPEN_APPEND | FA_WRITE);
		  if (fres == FR_OK)
		  {
			// write timestamp of this entry
			RTC_TimeStamp(currentDate,currentTime);
			UINT br = 0;
			logLen = snprintf(logBuf, 255, "(%s|%s)",currentDate,currentTime);
			f_write(&_DataFile, logBuf, logLen, &br);

			f_write(&_DataFile, data, size, &br);

			// echo data being written to debug port

#ifdef VERBOSE
			debugMessage(logBuf,logLen);

			logLen = snprintf(logBuf, 255, "... wrote <%s>\r\n",data);
			debugMessage(logBuf,logLen);
#endif
			// todo: also write above displayed message to status file
			f_close(&_DataFile);
			retval = br;	// number of bytes written for verification
		  }

	}
	return retval;
}

static int myFat_test(void)
{
	// send dummy byte
	SD_IO_WriteByte(0xFF);

	int retval = -1;

	if(BSP_SD_Init() != MSD_OK)
	{
		_Error_Handler(__FILE__, __LINE__);
	}

	/* Check the mounted device */
	if(WYT_fs_init() != FR_OK)
	{
		msglen = snprintf(msgBuf, 255, "ERR: fs_init() failed\r\n");
		debugMessage(msgBuf,msglen);

		_Error_Handler(__FILE__, __LINE__);
	}
	else{
		msglen = snprintf(msgBuf, 255, "fs_init() success\r\n");
		debugMessage(msgBuf,msglen);
		retval = OK;
	}

	return retval;

}

// format time and date for writing to a file, and (perhaps) for use with FAT
static int RTC_TimeStamp(char* stampDate, char* stampTime)
{
		HAL_RTC_GetTime(&hrtc, &sTime, RTC_FORMAT_BCD);
		HAL_RTC_GetDate(&hrtc, &sDate, RTC_FORMAT_BCD);

		msglen = snprintf(stampTime, 255, "%02d:%02d:%02d", sTime.Hours, sTime.Minutes, sTime.Seconds);
		msglen = snprintf(stampDate, 255, "%02d-%02d-%02d", sDate.Month, sDate.Date, sDate.Year);

		return 0;
}

/* realtime provider of dummy RFID tags */
static int dummyData(char* data, uint16_t size, uint8_t mode){

	uint8_t i;

	if (mode == MODE_TAG){
		// select 24 random nums between 0 and F
		for (i=0;i<24;i++){
			if (i==0){
				msglen = snprintf(data, 2, "%0X",
						data,
						(uint8_t) (rand() % 0xF));
			}
			msglen = snprintf(data, i, "%s%0X",
					data,
					(rand() % 0xF));
		}
		return 0;
	}

	else if (mode == MODE_RANDOM){
		// not implemented
		return 0;
	}

	return -1;
}


// todo: validate ALL needed files
static void ff_SDConfig(void)
{
	SD_IO_WriteByte(0xFF);

	if(FATFS_LinkDriver(&SD_Driver, SD_Path) == 0)
	{
		if(BSP_SD_Init() != MSD_OK)
		{
			_Error_Handler(__FILE__, __LINE__);
		}

		/* Check the mounted device */
		if(f_mount(&SD_FatFs, (TCHAR const*)"/", 1) != FR_OK)
		{
			_Error_Handler(__FILE__, __LINE__);
		}
		else
		{
			  fres = f_open(&_StatusFile, "ff_Status.txt", FA_CREATE_ALWAYS | FA_WRITE);
			  if (fres == FR_OK)
			  {
				UINT br = 0;
				for (int line = 0;  line < 5; ++line)
				{
					  msglen = snprintf(msgBuf, 255, "Test line %d\r\n",line);
					  f_write(&_StatusFile, msgBuf, msglen, &br);

					  if (br!= msglen){
						  _Error_Handler(__FILE__, __LINE__);
					  }
				}
				f_close(&_StatusFile);
			  }
		}
	}
}

/* USART channel over which data is requested and transmitted */
void DATA_USART1_Init(void){

	  husart_data1.Instance = USART1;
	  husart_data1.Init.BaudRate = 115200;
	  husart_data1.Init.WordLength = USART_WORDLENGTH_8B;
	  husart_data1.Init.StopBits = USART_STOPBITS_1;
	  husart_data1.Init.Parity = USART_PARITY_NONE;
	  husart_data1.Init.Mode = USART_MODE_TX_RX;
	  husart_data1.Init.CLKPolarity = USART_POLARITY_LOW;
	  husart_data1.Init.CLKPhase = USART_PHASE_1EDGE;
	  husart_data1.Init.CLKLastBit = USART_LASTBIT_DISABLE;
	  if (HAL_USART_Init(&husart_data1) != HAL_OK)
	  {
	    _Error_Handler(__FILE__, __LINE__);
	  }

}


