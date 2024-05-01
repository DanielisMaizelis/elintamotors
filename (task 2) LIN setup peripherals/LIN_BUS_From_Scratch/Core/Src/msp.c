#include <main.h>


void Msp_Init(void)
{
	__HAL_RCC_SYSCFG_CLK_ENABLE();
	__HAL_RCC_PWR_CLK_ENABLE();

	HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);
	SCB->SHCSR |= 0x7 << 16; // System exceptions for Usage Fault

	HAL_NVIC_EnableIRQ(UsageFault_IRQn);
	HAL_NVIC_EnableIRQ(BusFault_IRQn);
	HAL_NVIC_EnableIRQ(MemoryManagement_IRQn);

	HAL_NVIC_SetPriority(UsageFault_IRQn, 0, 0);
	HAL_NVIC_SetPriority(BusFault_IRQn, 0, 0);
	HAL_NVIC_SetPriority(MemoryManagement_IRQn, 0, 0);
}


void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{
	//CLK  UART
	__HAL_RCC_USART6_CLK_ENABLE();
	//CLK  GPIO
	__HAL_RCC_GPIOC_CLK_ENABLE();
	//GPIO Alternate Peripherals
	GPIO_InitTypeDef GPIO_init;
	GPIO_init.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_init.Pull = GPIO_PULLUP;
	GPIO_init.Pin = GPIO_PIN_7|GPIO_PIN_6;
	GPIO_init.Mode = GPIO_MODE_AF_PP;
	GPIO_init.Alternate = GPIO_AF8_USART6;

	//GPIO Init
	HAL_GPIO_Init(GPIOC, &GPIO_init);

	HAL_NVIC_EnableIRQ(USART6_IRQn);
	HAL_NVIC_SetPriority(USART6_IRQn, 15, 0);
}
