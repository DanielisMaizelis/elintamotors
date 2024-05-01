#include <main.h>

//-------------
void SCC(void);
void Uart_Init(void);
void Error_Handler(void);
void GPIO_Init(void);
//-------------

UART_HandleTypeDef huart6;

uint8_t str_rx[20]; // Received message
uint8_t flag = 0;


uint8_t CalcChecksum(uint8_t data[])
{
	uint16_t sum = 0;
	for(int i=0; i<2; i++) // Collects the sum
	{
		sum += data[i];
	}
	while(sum > 0xFF) // Gets the remainder value
	{
		sum-=0xFF;
	}
	return (uint8_t)(0xFF-sum); // Subtracts from 255
}


void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	uint8_t id = str_rx[2]; // 3rd array element contains the id of the frame
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7,1);
	if(id == 0xFA)
	{ // Sends back the LIN Frame
		uint8_t sync = 0x55;
		uint8_t data[] = {0xC1,0xA2};
		uint8_t checksum = CalcChecksum(data);
		HAL_LIN_SendBreak(&huart6);
		HAL_UART_Transmit(&huart6, &sync, 1, 10);
		HAL_UART_Transmit(&huart6, &id, 1, 10);
		HAL_UART_Transmit(&huart6, data, 2, 10);
		HAL_UART_Transmit(&huart6, &(checksum), 1, 10);

		flag = 1;
	}
	else if(id == 0x1A) // If the id is 0x1A
	{
		flag = 2;
	}
}


int main(void)
{
	HAL_Init();
	SCC(); // System Clock Config
	GPIO_Init();
	Uart_Init();


	while(1)
	{
		HAL_UART_Receive_IT(&huart6, str_rx, 20);
		HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_4);

		if(flag == 1)
		{
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4,1);
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4,0);
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7,1);
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2,1);
			HAL_Delay(100);
		}
		else if(flag == 2)
		{

			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7,1);
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2,1);
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4,1);
		}
		else if(flag == 0)
		{
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, 1);
		}

		HAL_Delay(100);
	}
	return 0;
}

void SCC(void) // System Clock Config
{
	RCC_OscInitTypeDef osc_init = {0};
	RCC_ClkInitTypeDef clk_init = {0};

	osc_init.OscillatorType = RCC_OSCILLATORTYPE_HSE;
	osc_init.HSEState = RCC_HSE_ON;
	//? Calibration value
	if (HAL_RCC_OscConfig(&osc_init) != HAL_OK)
	{
		Error_Handler();
	}
	__HAL_RCC_HSI_DISABLE();

	clk_init.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
	                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
	clk_init.SYSCLKSource = RCC_SYSCLKSOURCE_HSE;
	clk_init.AHBCLKDivider = RCC_SYSCLK_DIV1;
	clk_init.APB1CLKDivider = RCC_CFGR_PPRE1_DIV1;
	clk_init.APB2CLKDivider =  RCC_CFGR_PPRE1_DIV1;

	if (HAL_RCC_ClockConfig(&clk_init, FLASH_LATENCY_0) != HAL_OK)
	{
		Error_Handler();
	}
}

void Error_Handler(void)
{
	while(1);
}


void GPIO_Init(void)
{
	GPIO_InitTypeDef gpio_init = {0};

	__HAL_RCC_GPIOA_CLK_ENABLE();

	gpio_init.Pin = GPIO_PIN_7|GPIO_PIN_4|GPIO_PIN_2;
	gpio_init.Mode = GPIO_MODE_OUTPUT_PP;
	gpio_init.Pull = GPIO_NOPULL;
	gpio_init.Speed = GPIO_SPEED_FREQ_LOW;

	HAL_GPIO_Init(GPIOA,&gpio_init);
}

void Uart_Init(void)
{
	huart6.Instance = USART6; // Peripheral declaration
	huart6.Init.BaudRate = 19200;
	huart6.Init.WordLength = UART_WORDLENGTH_8B;
	huart6.Init.StopBits = UART_STOPBITS_1;
	huart6.Init.Parity = UART_PARITY_NONE;
	huart6.Init.Mode = UART_MODE_TX_RX;

	if(HAL_UART_Init(&huart6) != HAL_OK)
	{
		Error_Handler();
	}

	//	LIN init
	if(HAL_LIN_Init(&huart6, UART_LINBREAKDETECTLENGTH_11B) != HAL_OK)//-
	{
		Error_Handler();
	}
}

