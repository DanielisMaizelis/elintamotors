/*
 * it.c
 *
 *  Created on: Jan 27, 2023
 *      Author: Maizel
 */

#include <it.h>
#include <main.h>
extern UART_HandleTypeDef huart6;

void NMI_Handler(void)
{
  while(1);
}

void HardFault_Handler(void)
{
  while(1);
}

void MemManage_Handler(void)
{
  while(1);
}

void BusFault_Handler(void)
{
  while(1);
}

/**
  * @brief This function handles Undefined instruction or illegal state.
  */
void UsageFault_Handler(void)
{
  while(1);
}

void SVC_Handler(void)
{
}

void DebugMon_Handler(void)
{
}

void PendSV_Handler(void)
{
}

void SysTick_Handler(void)
{
	HAL_IncTick();
}


void USART6_IRQHandler(void)
{
	HAL_UART_IRQHandler(&huart6);
}
