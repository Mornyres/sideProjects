#include "global_config.h"

int debugMessage(char* msgBuf, int len, int DEBUGMETHOD){
	int retval;

	retval = HAL_USART_Transmit(&husart_debug, (uint8_t*)msgBuf, len, 0xFF);

	// or UART

	// blink LED
	// todo: generalize for any debug LED pin
	// in theory if you're not plugged in for HAL_UART_Transmit it'll complain but you can loosely interpret the message
	for (int i = 0; i<len;i++){
		HAL_GPIO_TogglePin(GREENLED_Port, GREENLED_Pin);
		HAL_Delay(10);
		HAL_GPIO_TogglePin(GREENLED_Port, GREENLED_Pin);
	}

	HAL_Delay(5);
	return retval;
}

/* general use debug USART -- usb for disco board, but leaving it available for future too */
void DEBUG_USART_Init(void){

	  husart_debug.Instance = USART2;
	  husart_debug.Init.BaudRate = 115200;
	  husart_debug.Init.WordLength = USART_WORDLENGTH_8B;
	  husart_debug.Init.StopBits = USART_STOPBITS_1;
	  husart_debug.Init.Parity = USART_PARITY_NONE;
	  husart_debug.Init.Mode = USART_MODE_TX_RX;
	  husart_debug.Init.CLKPolarity = USART_POLARITY_LOW;
	  husart_debug.Init.CLKPhase = USART_PHASE_1EDGE;
	  husart_debug.Init.CLKLastBit = USART_LASTBIT_DISABLE;
	  if (HAL_USART_Init(&husart_debug) != HAL_OK)
	  {
	    _Error_Handler(__FILE__, __LINE__);
	  }

}


void _Error_Handler(char *file, int line)
{

	int msglen = 0;
	char msgBuf[255] = "\0";

	msglen = snprintf(msgBuf, 255, "ERR: %s:%d\r\n", file, line);
	debugMessage(msgBuf,msglen);

	HAL_GPIO_TogglePin(REDLED_Port, REDLED_Pin);
	HAL_Delay(10);
	  // for debugging, just continue through an error to see what else might go wrong
#if IGNORE_ERRORS != 1
	  while(1){}
#endif
	HAL_GPIO_TogglePin(REDLED_Port, REDLED_Pin);
	msglen = snprintf(msgBuf, 255, "ERR: Continuing anyway...\r\n");
	debugMessage(msgBuf,msglen);
}

