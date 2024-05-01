#include "main.h"
#include <stdbool.h>

TIM_HandleTypeDef htim6;
TIM_HandleTypeDef htim7;
TIM_HandleTypeDef htim10;
TIM_HandleTypeDef htim11;

UART_HandleTypeDef huart6;

void systemClockConfig(void);
static void gpioInit(void);
static void tim6Init(void);
static void usart6Init(void);
static void tim7Init(void);
static void tim10Init(void);

uint8_t Normal_checksum(uint8_t data[]);
void ledToggling(uint8_t red_led_value, uint8_t green_led_value);
void linStateSwitching(void);
void linCorrectMsgIdCheck(void);
void checkUartStaus(uint32_t *pUartDisconnectedCounter, uint8_t linMessageRx[]);

/*
* 'LIN_INCORRECT_MSG_LIMIT' helps recognise, when the message is incorrect,
* it also helps skip out timing errors
*/
#define LIN_INCORRECT_MSG_LIMIT 12000

// Correct id of the LIN message
#define LIN_RESPONSE_MSG_ID 0xAA
#define LIN_MSG_HEADER_ID 0xBB
/*
* The 'UART_DISCONNECTED_LIMIT' helps indicate that to this certain time frame
* there should not be UART reception
*/
#define UART_DISCONNECTED_LIMIT 45000

// LIN States That will light up LED light color in accordance to the LIN connection status
typedef enum
{
  LinState_LIN_OFF, // LED is blinking
  LinState_UART_CONNECT_FAIL, // LED is red
  LinState_LIN_INCORRECT_MSG, // LED is red
  LinState_LIN_SUCCESS // LED is green
} LinState_e;

// Start of te program, when the button is not pressd and Red ang Green LEDs are toggled
typedef enum
{
  LinOffLedToggle_RED_LED_ON,
  LinOffLedToggle_GREEN_LED_ON
} LinOffLedToggle_e;

LinState_e gLinState               = LinState_LIN_OFF; // Default value for Enum
LinOffLedToggle_e gLinOffLedToggle = LinOffLedToggle_RED_LED_ON; // Default value for Enum


/*
* 'gButtonPressLedsOff' - intentional non blocking delay, this helps the user to see if the
* button is pressed.
* Also when the big leds are off for the brief while, it then resets the LIN state.
*/
bool     gButtonPressLedsOff         = true;
bool     gDebounceDelayTim6          = false;
bool     gUartStatusOk               = false;
uint32_t gLinIncorrectMsgCounter     = 0;
uint8_t  gLinRxMsgId = 0x00;

// Button interrupt, which will change states for LIN
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  // If the button press is recognised and the debounce is not triggered at the moment
  if(GPIO_Pin == GPIO_PIN_7 && gDebounceDelayTim6 == false)
  {
    gDebounceDelayTim6 = true; // Triggers the debounce delay
    HAL_TIM_Base_Start_IT(&htim6); // Starts a short period timer for debounce
  }
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  // 1 second non blocking delay
  if(htim->Instance == TIM7)
  {
    // When the button is not pressed:
    if(gLinState == LinState_LIN_OFF)
    {
      // This will toggle the LED Green and Red:
      // This tells the user that the button was not pressed yet.
      switch(gLinOffLedToggle)
      {
      case LinOffLedToggle_RED_LED_ON:
        ledToggling(1, 0); // Turns the LED red
        gLinOffLedToggle = LinOffLedToggle_GREEN_LED_ON;
        break;
      case LinOffLedToggle_GREEN_LED_ON:
        ledToggling(0, 1);  // Turns the LED green
        gLinOffLedToggle = LinOffLedToggle_RED_LED_ON;
        break;
      }
    }
    // This will toggle the internal LED which will show that the CPU is working correctly.
    HAL_GPIO_TogglePin(GPIOA, CPU_STATUS_Pin);
  }

  // Debounce non blocking delay
  if(htim->Instance == TIM6)
  {
    if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_7) == 0)
    {
      gDebounceDelayTim6 = false;
      gLinState          = LinState_UART_CONNECT_FAIL; // Sets default state when LIN is connected
      gButtonPressLedsOff= true; // Temporarily turns off all LEDs
      HAL_TIM_Base_Stop_IT(&htim6);
      HAL_TIM_Base_Start_IT(&htim10);
    }
  }

  // IRQ to turn the big LED off (non blocking delay) - this means that
  // all lights are temporarily turned off for a second, signaling that the button is pressed
  if(htim->Instance == TIM10)
  {
    gButtonPressLedsOff = false; // Turns the
    HAL_TIM_Base_Stop_IT(&htim10);
  }
}

// When the UART callback is received
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
  gUartStatusOk = true; // Flag for telling that the Uart connection is good
  HAL_GPIO_WritePin(GPIOA, UART_STATUS_Pin, 1); // This LED tells me that UART is working
  if(gLinRxMsgId != LIN_MSG_HEADER_ID && gLinRxMsgId != LIN_RESPONSE_MSG_ID)
  {
    gLinIncorrectMsgCounter++; // If the ID when receiving the UART is incorrect:
  }
}

#if 0
// Calculates LIN checksum
uint8_t normalChecksum(uint8_t data[])
{
  uint16_t sum = 0;
  for(int i = 0; i < 2; i++) // Collects the sum
  {
    sum += data[i];
  }
  while(sum > 0xFF) // Gets the remainder value
  {
    sum -= 0xFF;
  }
  return (uint8_t) (0xFF - sum); // Subtracts from 255
}
#endif

void ledToggling(uint8_t redLedValue, uint8_t greenLedValue)
{
  // This will temporarily turn off the LED with the press of the button
  if(gButtonPressLedsOff && gLinState != LinState_LIN_OFF)
  {
    HAL_GPIO_WritePin(GPIOB, RED_LED_Pin, 0);
    HAL_GPIO_WritePin(GPIOB, GREEN_LED_Pin, 0);
  }
  // It will toggle LEDs when the Button interrupt is not triggered
  // (that briefly turns the LED off).
  else if(!gButtonPressLedsOff || gLinState == LinState_LIN_OFF)
  {
    HAL_GPIO_WritePin(GPIOB, RED_LED_Pin, redLedValue);
    HAL_GPIO_WritePin(GPIOB, GREEN_LED_Pin, greenLedValue);
  }
}

// Based on the state it will turn the corresponding LEDs on
void linStateSwitching(void)
{
  switch(gLinState)
  {
  case LinState_UART_CONNECT_FAIL: // When there isn't a UART/LIN connection
    ledToggling(1, 0); // LED turns Red
    break;
  case LinState_LIN_INCORRECT_MSG: // If the message is not valid by certain parameters (id)
    ledToggling(1, 0); // LED turns Red
    break;
  case LinState_LIN_SUCCESS: // If LIN message applies to the parameters and the connection is good
    ledToggling(0, 1); // LED turns Green
    break;
  }
}


void linCorrectMsgIdCheck(void)
{
  // If the message is correct
  if(gLinRxMsgId == LIN_RESPONSE_MSG_ID || gLinRxMsgId == LIN_MSG_HEADER_ID  /*&& gLinIncorrectMsgCounter <= LIN_INCORRECT_MSG_LIMIT*/)
  {
    gLinState = LinState_LIN_SUCCESS; // Tells that the LIN message is receiving correctly
    gLinIncorrectMsgCounter = 0; // Resets the error message id counter
  }
  // If the message id is incorrect for a certain period of time
  else if(gLinRxMsgId != LIN_RESPONSE_MSG_ID && gLinRxMsgId != LIN_MSG_HEADER_ID && gLinIncorrectMsgCounter > LIN_INCORRECT_MSG_LIMIT)
  {
    gLinState = LinState_LIN_INCORRECT_MSG; // Tells that the LIN message is receiving incorrectly
  }
}

/*
* Checks UART connection status, if the message is not received after 'UART_DISCONNECTED_LIMIT'
*  This will then turn the red LED on.
*/
void checkUartStaus(uint32_t *pUartDisconnectedCounter, uint8_t linMessageRx[])
{
  // Default LIN message id value
  gLinRxMsgId = 0x00;
  // This means that connection status for uart is good
  if(gUartStatusOk)
  {
    gUartStatusOk               = false; // Turns back to false to constantly check UART status
    gLinRxMsgId                 = linMessageRx[2]; // 3rd array element contains the id of the frame
    *pUartDisconnectedCounter   = 0;
    // Checks if lin message id is valid
    linCorrectMsgIdCheck();
  }
  // Counts up how long the Idle state is, because the UART connection is off:
  else
  {
    *pUartDisconnectedCounter += 1;
    if(*pUartDisconnectedCounter >= UART_DISCONNECTED_LIMIT)
    {
      gLinState = LinState_UART_CONNECT_FAIL;
      // Resets the counter for failed LIN message, since no UART reception is happening at the moment
      gLinIncorrectMsgCounter = 0;
      HAL_GPIO_WritePin(GPIOA, UART_STATUS_Pin, 0);
    }
  }
}

int main(void)
{
  HAL_Init();
  systemClockConfig();
  gpioInit();
  tim6Init();
  usart6Init();
  tim7Init();
  tim10Init(); // Timer 10 is used for
  HAL_TIM_Base_Start_IT(&htim7);

  uint8_t  linMessageRx[20]; // Received message
  uint32_t uartDisconnectedCounter = 0;
  while(1)
  {
    if(gLinState != LinState_LIN_OFF)
    {
      // Tries to receive the UART message
      HAL_UART_Receive_IT(&huart6, linMessageRx, 20);
      // Checks if the UART connection is good
      checkUartStaus(&uartDisconnectedCounter,linMessageRx);
      // Switches LEDs is accordance to the applied state
      linStateSwitching();
    }
  }
}



// MX generated Inits
void systemClockConfig(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if(HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSE;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if(HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

static void tim6Init(void)
{
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  htim6.Instance = TIM6;
  htim6.Init.Prescaler = 11999;
  htim6.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim6.Init.Period = 49;
  htim6.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;

  if(HAL_TIM_Base_Init(&htim6) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;

  if(HAL_TIMEx_MasterConfigSynchronization(&htim6, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
}

static void tim7Init(void)
{
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  htim7.Instance = TIM7;
  htim7.Init.Prescaler = 11999;
  htim7.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim7.Init.Period = 199;
  htim7.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;

  if(HAL_TIM_Base_Init(&htim7) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;

  if(HAL_TIMEx_MasterConfigSynchronization(&htim7, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
}

static void tim10Init(void)
{
  htim10.Instance = TIM10;
  htim10.Init.Prescaler = 11999;
  htim10.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim10.Init.Period = 999;
  htim10.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim10.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;

  if(HAL_TIM_Base_Init(&htim10) != HAL_OK)
  {
    Error_Handler();
  }
}

static void usart6Init(void)
{
  huart6.Instance = USART6;
  huart6.Init.BaudRate = 19200;
  huart6.Init.WordLength = UART_WORDLENGTH_8B;
  huart6.Init.StopBits = UART_STOPBITS_1;
  huart6.Init.Parity = UART_PARITY_NONE;
  huart6.Init.Mode = UART_MODE_TX_RX;
  huart6.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart6.Init.OverSampling = UART_OVERSAMPLING_16;

  if(HAL_LIN_Init(&huart6, UART_LINBREAKDETECTLENGTH_11B) != HAL_OK)
  {
    Error_Handler();
  }
}

static void gpioInit(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, RED_LED_Pin|GREEN_LED_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, CPU_STATUS_Pin|UART_STATUS_Pin|GPIO_PIN_2, GPIO_PIN_RESET);

  /*Configure GPIO pin : PB7 */
  GPIO_InitStruct.Pin = GPIO_PIN_7;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : RED_LED_Pin GREEN_LED_Pin */
  GPIO_InitStruct.Pin = RED_LED_Pin|GREEN_LED_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : CPU_STATUS_Pin UART_STATUS_Pin PA2 */
  GPIO_InitStruct.Pin = CPU_STATUS_Pin|UART_STATUS_Pin|GPIO_PIN_2;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI9_5_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);

}

void Error_Handler(void)
{
  while(1)
  {
  }
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
