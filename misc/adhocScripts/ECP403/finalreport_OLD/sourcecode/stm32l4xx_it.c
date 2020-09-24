/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32l4xx_it.h"
#include "stm32l4xx_hal.h"

#include "global_config.h"

/******************************************************************************/
/*            Cortex-M4 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(unsigned int * hardfault_args)
{
  /* USER CODE BEGIN HardFault_IRQn 0 */
	// Following Joseph Yiu's guides on hardfault handling ---
	// as well as http://www.keil.com/appnotes/files/apnt209.pdf
	// https://community.arm.com/cfs-file/__key/telligent-evolution-components-attachments/01-2057-00-00-00-01-28-35/Cortex_2D00_M-for-Beginners-_2D00_-2017_5F00_EN_5F00_v2.pdf
	  unsigned int stacked_r0;
	  unsigned int stacked_r1;
	  unsigned int stacked_r2;
	  unsigned int stacked_r3;
	  unsigned int stacked_r12;
	  volatile unsigned int stacked_lr;
	  volatile unsigned int stacked_pc;
	  volatile unsigned int stacked_psr;

	  stacked_r0 = ((unsigned long) hardfault_args[0]);
	  stacked_r1 = ((unsigned long) hardfault_args[1]);
	  stacked_r2 = ((unsigned long) hardfault_args[2]);
	  stacked_r3 = ((unsigned long) hardfault_args[3]);

	  stacked_r12 = ((unsigned long) hardfault_args[4]);
	  stacked_lr = ((unsigned long) hardfault_args[5]);
	  stacked_pc = ((unsigned long) hardfault_args[6]);
	  stacked_psr = ((unsigned long) hardfault_args[7]);


	  msglen = snprintf(msgBuf, 255,"[HARDFAULT]\r\n");
	  HAL_USART_Transmit(&husart_debug, (uint8_t*)msgBuf, msglen, 0xFF);

	  msglen = snprintf(msgBuf, 255,"R0 = %x\r\n", stacked_r0);
	  HAL_USART_Transmit(&husart_debug, (uint8_t*)msgBuf, msglen, 0xFF);


	  msglen = snprintf(msgBuf, 255,"R1 = %x\r\n", stacked_r1);
	  HAL_USART_Transmit(&husart_debug, (uint8_t*)msgBuf, msglen, 0xFF);

	  msglen = snprintf(msgBuf, 255,"R2 = %x\r\n", stacked_r2);
	  HAL_USART_Transmit(&husart_debug, (uint8_t*)msgBuf, msglen, 0xFF);

	  msglen = snprintf(msgBuf, 255,"R3 = %x\r\n", stacked_r3);
	  HAL_USART_Transmit(&husart_debug, (uint8_t*)msgBuf, msglen, 0xFF);

	  msglen = snprintf(msgBuf, 255,"R12 = %x\r\n", stacked_r12);
	  HAL_USART_Transmit(&husart_debug, (uint8_t*)msgBuf, msglen, 0xFF);

	  msglen = snprintf(msgBuf, 255,"LR [R14] = %x  subroutine call return address\r\n", stacked_lr);
	  HAL_USART_Transmit(&husart_debug, (uint8_t*)msgBuf, msglen, 0xFF);

	  msglen = snprintf(msgBuf, 255,"PC [R15] = %x  program counter\r\n", stacked_pc);
	  HAL_USART_Transmit(&husart_debug, (uint8_t*)msgBuf, msglen, 0xFF);

	  /* Execution program status register*/
	  msglen = snprintf(msgBuf, 255,"PSR = %x\r\n", stacked_psr);
	  HAL_USART_Transmit(&husart_debug, (uint8_t*)msgBuf, msglen, 0xFF);

	  /*Config/Control register  Fault Status Register*/
	  msglen = snprintf(msgBuf, 255,"CCR = %08lX\r\n", SCB->CCR);
	  HAL_USART_Transmit(&husart_debug, (uint8_t*)msgBuf, msglen, 0xFF);

	  /*System Handler Control and State Register */
	  msglen = snprintf(msgBuf, 255,"SHCSR = %08lX\r\n", SCB->SHCSR);
	  HAL_USART_Transmit(&husart_debug, (uint8_t*)msgBuf, msglen, 0xFF);

	  /*Bus Fault Status Register*/
	  msglen = snprintf(msgBuf, 255,"BFSR = %04X\r\n",  (*((volatile uint8_t*)(0xE000ED29))));
	  HAL_USART_Transmit(&husart_debug, (uint8_t*)msgBuf, msglen, 0xFF);

	  /*Usage Fault Status Register*/
	  msglen = snprintf(msgBuf, 255,"UFSR = %04X\r\n",  (*((volatile uint8_t*)(0xE000ED2A))));
	  HAL_USART_Transmit(&husart_debug, (uint8_t*)msgBuf, msglen, 0xFF);

	  /* HardFault Status Register */
	  msglen = snprintf(msgBuf, 255,"HFSR = %08lX\r\n", SCB->HFSR);
	  HAL_USART_Transmit(&husart_debug, (uint8_t*)msgBuf, msglen, 0xFF);

	  /* Config. Fault Status register */
	  msglen = snprintf(msgBuf, 255,"SCB_CFSR = %08lX\r\n",SCB->CFSR);
	  HAL_USART_Transmit(&husart_debug, (uint8_t*)msgBuf, msglen, 0xFF);



  /* USER CODE END HardFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_HardFault_IRQn 0 */
	/* USER CODE END W1_HardFault_IRQn 0 */
  }
  /* USER CODE BEGIN HardFault_IRQn 1 */

  /* USER CODE END HardFault_IRQn 1 */
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(unsigned int * memfault_args)
{
	 unsigned int stacked_pc = ((unsigned long) memfault_args[6]);
	 unsigned int stacked_lr = ((unsigned long) memfault_args[5]);

 /* USER CODE BEGIN MemoryManagement_IRQn 0 */
	  msglen = snprintf(msgBuf, 255,"[MMFAULT]\r\n");
	  HAL_USART_Transmit(&husart_debug, (uint8_t*)msgBuf, msglen, 0xFF);

	  msglen = snprintf(msgBuf, 255,"LR [R14] = %x  subroutine call return address\r\n", stacked_lr);
	  HAL_USART_Transmit(&husart_debug, (uint8_t*)msgBuf, msglen, 0xFF);

	  msglen = snprintf(msgBuf, 255,"PC [R15] = %x  program counter\r\n", stacked_pc);
	  HAL_USART_Transmit(&husart_debug, (uint8_t*)msgBuf, msglen, 0xFF);

	  /*MemManage Fault Status Register:wq*/
	  msglen = snprintf(msgBuf, 255,"MMFSR = %04X\r\n",  (*((volatile uint8_t*)(0xE000ED28))));
	  HAL_USART_Transmit(&husart_debug, (uint8_t*)msgBuf, msglen, 0xFF);

	  msglen = snprintf(msgBuf, 255,"MMFAR = %08X\r\n",  (*((volatile uint8_t*)(0xE000ED34))));
	  HAL_USART_Transmit(&husart_debug, (uint8_t*)msgBuf, msglen, 0xFF);

	  /*System Handler Control and State Register */
	  msglen = snprintf(msgBuf, 255,"SHCSR = %08lX\r\n", SCB->SHCSR);
	  HAL_USART_Transmit(&husart_debug, (uint8_t*)msgBuf, msglen, 0xFF);

 /* USER CODE END MemoryManagement_IRQn 0 */
 while (1)
 {
   /* USER CODE BEGIN W1_MemoryManagement_IRQn 0 */
   /* USER CODE END W1_MemoryManagement_IRQn 0 */
 }
 /* USER CODE BEGIN MemoryManagement_IRQn 1 */

 /* USER CODE END MemoryManagement_IRQn 1 */
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{

  /* USER CODE BEGIN BusFault_IRQn 0 */
	  msglen = snprintf(msgBuf, 255,"[BUSFAULT]\r\n");
	  HAL_USART_Transmit(&husart_debug, (uint8_t*)msgBuf, msglen, 0xFF);


  /* USER CODE END BusFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_BusFault_IRQn 0 */
    /* USER CODE END W1_BusFault_IRQn 0 */
  }
  /* USER CODE BEGIN BusFault_IRQn 1 */

  /* USER CODE END BusFault_IRQn 1 */
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
    HAL_IncTick();
}

/******************************************************************************/
/*                 STM32L4xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32l4xx.s).                                               */
/******************************************************************************/

/**
  * @brief  This function handles lines 10 to 15 interrupt request.
  * @param  None
  * @retval None
  */
void EXTI15_10_IRQHandler(void)
{
  HAL_GPIO_EXTI_IRQHandler(USER_BUTTON_PIN);
}

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
