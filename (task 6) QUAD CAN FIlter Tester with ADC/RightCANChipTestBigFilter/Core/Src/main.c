/* USER CODE BEGIN Header */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
ADC_HandleTypeDef hadc1;

CAN_HandleTypeDef hcan1;
CAN_HandleTypeDef hcan2;

TIM_HandleTypeDef htim6;
TIM_HandleTypeDef htim7;

UART_HandleTypeDef huart1;
UART_HandleTypeDef huart2;
DMA_HandleTypeDef hdma_usart1_rx;

/* USER CODE BEGIN PV */
uint8_t gUartByte;              // Received data byte from uart
uint8_t gCanReceivedMsg = 0x00; // Received CAN message from an interrupt
uint8_t gChannelsTested;
uint8_t gAdcRxDataUart[TOTAL_ADC_CONN_C][ADC_DATA_LEN]; // Stored adc data received from Uart

bool    gStartDmaUart   = false; // A boolean for starting uart dma reception
bool    gAllAdcReceived = false; // Tells if all the bytes for adc have been received
uint8_t turnOn          = 0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_CAN1_Init(void);
static void MX_CAN2_Init(void);
static void MX_ADC1_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_TIM7_Init(void);
static void MX_TIM6_Init(void);
/* USER CODE BEGIN PFP */
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

// Button interrupt, which will trigger the reset.
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  // If the button press is recognised and the debounce is not triggered at the moment
  if(GPIO_Pin == GPIO_PIN_5)
  {
    HAL_TIM_Base_Start_IT(&htim6); // Starts a short period timer for debounce
  }
}

// Timer callback irq function:
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  // Timer for Debounce:
  if(htim->Instance == TIM6)
  {
    if(turnOn == 2)
    {
      NVIC_SystemReset();
    }
    else
    {
      turnOn = 1;
    }
    HAL_TIM_Base_Stop_IT(&htim6); // Stops the timer
  }
  // Timer for Toggling the builtin LED:
  if(htim->Instance == TIM7)
  {
    HAL_GPIO_TogglePin(GPIOC, LED3_Pin);
  }
}
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */
  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */
  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_CAN1_Init();
  MX_CAN2_Init();
  MX_ADC1_Init();
  MX_USART1_UART_Init();
  MX_USART2_UART_Init();
  MX_TIM7_Init();
  MX_TIM6_Init();
  /* USER CODE BEGIN 2 */
  while(turnOn != 1){}
  HAL_UART_Transmit(&huart1,(uint8_t*)"A", 1, 10);
  uint8_t C_CheckCount;       // Counter for tested conncetions from the C module
  uint8_t C1_CheckCount;      // Counter for tested conncetions from the C1 module
  uint8_t totalConnCount;     // Total connections tested (C1 and C combined)
  bool    bTestPass = true;

  // These are the main program states:
  MainStates_e  mainState  = MainStates_CHECK_CONNECTIONS;

  // Connections States, telling if the connection being tested is from C or from C1 module:
  ModuleTested_e moduleTested;

  // Test stages for each connection test
  TestStages_e testStage;

  // Data from C Module:
  C_Connections_t C_Connection[3]; // "C" MCU struct:
  C_Connection[0] = C_Init((uint8_t*)CAN1_NAME, C_Connection[0]); // CAN 1 parameters (UART):
  C_Connection[1] = C_Init((uint8_t*)CAN2_NAME, C_Connection[1]); // CAN 2 parameters (UART):
  C_Connection[2] = C_Init((uint8_t*)LIN_NAME, C_Connection[2]);  // LIN parameters (UART):

  // Data from this - C1 Module:
  C1_Connections_t C1_Connection[2]; // "C1" Connections MCU struct:
  C1_Connection[0] = C1_Init(&hcan1, (uint8_t*)CAN3_NAME, C1_Connection[0]); // CAN3 Parameters:
  C1_Connection[1] = C1_Init(&hcan2, (uint8_t*)CAN4_NAME, C1_Connection[1]); // CAN4 Parameters:

  test1  test1[6];
  for(int i=0; i<6 ; i++)
  {
    test1[i].bCurrentOk = true;
  }

  print_startMenu(); // Print start menu in the terminal:
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  HAL_GPIO_WritePin(GPIOA,PASS_C1_Pin, 0);
  HAL_GPIO_WritePin(GPIOA,FAIL_C1_Pin, 0);

  gChannelsTested = 2;
  uartRx_receiveAdcDataC(&huart1);
  print_adcDataC(gAdcRxDataUart, test1, gChannelsTested);

  memset(gAdcRxDataUart, 0, sizeof(gAdcRxDataUart));      // Resets the global variable array
  gStartDmaUart = true;                                   // Starts the dma reading UART

  for(uint8_t testTry=1; testTry<=2; testTry++) // 2 test tries are used to fully test the system without any hiccups
  {
    // Resets all the counters and states for a new try
    C_CheckCount   = 0;
    C1_CheckCount  = 0;
    totalConnCount = 0;
    moduleTested   = ModuleTested_CHECK_C_UART;
    testStage      = TestStages_TRANSMIT_MSG;
    do
    {
      switch(mainState)
      {
          case MainStates_CHECK_CONNECTIONS:
            // If the Id is not for CAN1 and CAN2
            switch(moduleTested)
            {
              case ModuleTested_CHECK_C_UART:
                switch(testStage)
                {
                   case TestStages_TRANSMIT_MSG: // Enters the transmit stage
                     HAL_UART_Transmit(&huart1, &C_Connection[C_CheckCount].uartMsg, 1, 10);
                     testStage = TestStages_RECEIVE; // Goes to the next stage

                   case TestStages_RECEIVE: // Enters the reception stage
                     // The byte received will then return the status of the connection from this funtion (Good or Bad)
                     C_Connection[C_CheckCount].bIsConnected = uartRx_isConnectionOk(C_Connection[C_CheckCount].uartMsg, C_Connection[C_CheckCount].uartMsgError);
                     testStage = TestStages_TRANSMIT_MSG; // Goes to the first stage
                     break;
                }
                break;

              case ModuleTested_CHECK_C1:
                switch(testStage)
                {
                  case TestStages_TRANSMIT_MSG: // Enters the transmit stage
                    canTx_sendMsg(C1_Connection[C1_CheckCount].canId, C1_Connection[C1_CheckCount].canMsg, C1_Connection[C1_CheckCount].pCan);
                    testStage = TestStages_RECEIVE; // Goes to the next stage

                  case TestStages_RECEIVE: // Enters the reception stage
                    // This returns the checked status of the connetion (Good or Bad)
                    C1_Connection[C1_CheckCount].bIsConnected = canRx_isCanOk(C1_Connection[C1_CheckCount].canMsg);
                    testStage = TestStages_TRANSMIT_MSG;  // Goes to the first stage
                 }
                break;
            }
            mainState = MainStates_PRINT_STATUS_USB; // Moves on to the next state:
            break;
          case MainStates_PRINT_STATUS_USB:
            // Print status Uart for CAN1 and CAN2
            switch(moduleTested)
            {
             case ModuleTested_CHECK_C_UART:
                 if(testTry == 2)  // Print status Uart for CAN3 and CAN4 on a second test try
                 {
                   // Prints the C connection data onto the terminal
                   print_connectionStatus(totalConnCount, C_Connection[C_CheckCount].name, C_Connection[C_CheckCount].bIsConnected, &huart2);
                 }
                 C_CheckCount++;
                 break;
              case ModuleTested_CHECK_C1:
                if(testTry == 2)  // Print status Uart for CAN1 and CAN2, LIN on a second test try
                {
                  // Prints the C1 connection data onto the terminal
                  print_connectionStatus(totalConnCount, C1_Connection[C1_CheckCount].name, C1_Connection[C1_CheckCount].bIsConnected, &huart2);
                }
                C1_CheckCount++;
                break;
            }
            totalConnCount++;       // Increments the total connections tested
            if(totalConnCount == 3) // If CAN1, CAN2 and LIN have already been tested:
            {
              moduleTested = ModuleTested_CHECK_C1;   // Move to testing the C module connections
            }
            mainState = MainStates_CHECK_CONNECTIONS; // Moves on to the primary state:
            break;
      }
    }
    while(totalConnCount < TOTAL_CONNECTIONS); // Checks if all connections (C and C1) have been tested
    }
    gStartDmaUart   = false;
    HAL_UART_Transmit(&huart1,(uint8_t*)"R", 1, 10);


    gChannelsTested = 3;
    uartRx_receiveAdcDataC(&huart1);
    print_adcDataC(gAdcRxDataUart, test1, gChannelsTested);

    //--
    for(int i = 0; i<3;i++)
    {
      if(C_Connection[i].bIsConnected == false)
      {
        bTestPass = false;
      }
    }
    for(int i = 0; i<2; i++)
    {
      if(C1_Connection[i].bIsConnected == false)
      {
        bTestPass = false;
      }
    }
    print_endNote(bTestPass); // Prints the end data in the terminal
    //--

    turnOn = 2;
    while(1);
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  RCC_OscInitStruct.PLL2.PLL2State = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSE;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC;
  PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV6;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure the Systick interrupt time
  */
  __HAL_RCC_PLLI2S_ENABLE();
}

/**
  * @brief ADC1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_ADC1_Init(void)
{

  /* USER CODE BEGIN ADC1_Init 0 */

  /* USER CODE END ADC1_Init 0 */

  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC1_Init 1 */

  /* USER CODE END ADC1_Init 1 */

  /** Common config
  */
  hadc1.Instance = ADC1;
  hadc1.Init.ScanConvMode = ADC_SCAN_DISABLE;
  hadc1.Init.ContinuousConvMode = DISABLE;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.NbrOfConversion = 1;
  if (HAL_ADC_Init(&hadc1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Regular Channel
  */
  sConfig.Channel = ADC_CHANNEL_8;
  sConfig.Rank = ADC_REGULAR_RANK_1;
  sConfig.SamplingTime = ADC_SAMPLETIME_1CYCLE_5;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC1_Init 2 */

  /* USER CODE END ADC1_Init 2 */

}

/**
  * @brief CAN1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_CAN1_Init(void)
{

  /* USER CODE BEGIN CAN1_Init 0 */

  /* USER CODE END CAN1_Init 0 */

  /* USER CODE BEGIN CAN1_Init 1 */

  /* USER CODE END CAN1_Init 1 */
  hcan1.Instance = CAN1;
  hcan1.Init.Prescaler = 1;
  hcan1.Init.Mode = CAN_MODE_NORMAL;
  hcan1.Init.SyncJumpWidth = CAN_SJW_1TQ;
  hcan1.Init.TimeSeg1 = CAN_BS1_12TQ;
  hcan1.Init.TimeSeg2 = CAN_BS2_2TQ;
  hcan1.Init.TimeTriggeredMode = DISABLE;
  hcan1.Init.AutoBusOff = DISABLE;
  hcan1.Init.AutoWakeUp = DISABLE;
  hcan1.Init.AutoRetransmission = ENABLE;
  hcan1.Init.ReceiveFifoLocked = DISABLE;
  hcan1.Init.TransmitFifoPriority = DISABLE;
  if (HAL_CAN_Init(&hcan1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN CAN1_Init 2 */

  /* USER CODE END CAN1_Init 2 */

}

/**
  * @brief CAN2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_CAN2_Init(void)
{

  /* USER CODE BEGIN CAN2_Init 0 */

  /* USER CODE END CAN2_Init 0 */

  /* USER CODE BEGIN CAN2_Init 1 */

  /* USER CODE END CAN2_Init 1 */
  hcan2.Instance = CAN2;
  hcan2.Init.Prescaler = 1;
  hcan2.Init.Mode = CAN_MODE_NORMAL;
  hcan2.Init.SyncJumpWidth = CAN_SJW_1TQ;
  hcan2.Init.TimeSeg1 = CAN_BS1_12TQ;
  hcan2.Init.TimeSeg2 = CAN_BS2_2TQ;
  hcan2.Init.TimeTriggeredMode = DISABLE;
  hcan2.Init.AutoBusOff = DISABLE;
  hcan2.Init.AutoWakeUp = DISABLE;
  hcan2.Init.AutoRetransmission = DISABLE;
  hcan2.Init.ReceiveFifoLocked = DISABLE;
  hcan2.Init.TransmitFifoPriority = DISABLE;
  if (HAL_CAN_Init(&hcan2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN CAN2_Init 2 */

  /* USER CODE END CAN2_Init 2 */

}

/**
  * @brief TIM6 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM6_Init(void)
{

  /* USER CODE BEGIN TIM6_Init 0 */

  /* USER CODE END TIM6_Init 0 */

  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM6_Init 1 */

  /* USER CODE END TIM6_Init 1 */
  htim6.Instance = TIM6;
  htim6.Init.Prescaler = 11999;
  htim6.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim6.Init.Period = 1;
  htim6.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim6) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim6, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM6_Init 2 */

  /* USER CODE END TIM6_Init 2 */

}

/**
  * @brief TIM7 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM7_Init(void)
{

  /* USER CODE BEGIN TIM7_Init 0 */

  /* USER CODE END TIM7_Init 0 */

  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM7_Init 1 */

  /* USER CODE END TIM7_Init 1 */
  htim7.Instance = TIM7;
  htim7.Init.Prescaler = 12000;
  htim7.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim7.Init.Period = 100;
  htim7.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
  if (HAL_TIM_Base_Init(&htim7) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim7, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM7_Init 2 */
  HAL_TIM_Base_Start_IT(&htim7);
  /* USER CODE END TIM7_Init 2 */

}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 9600;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 9600;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void)
{

  /* DMA controller clock enable */
  __HAL_RCC_DMA1_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA1_Channel5_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Channel5_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel5_IRQn);

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, LED1_Pin|LED2_Pin|LED3_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, PASS_C1_Pin|FAIL_C1_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : LED1_Pin LED2_Pin LED3_Pin */
  GPIO_InitStruct.Pin = LED1_Pin|LED2_Pin|LED3_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pin : PA5 */
  GPIO_InitStruct.Pin = GPIO_PIN_5;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING_FALLING;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : PASS_C1_Pin FAIL_C1_Pin */
  GPIO_InitStruct.Pin = PASS_C1_Pin|FAIL_C1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI9_5_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);

}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

  HAL_GPIO_WritePin(GPIOC, LED1_Pin | LED2_Pin | LED3_Pin, 0);
  while(1)
  {
    HAL_GPIO_WritePin(GPIOC, LED1_Pin, 1);
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
