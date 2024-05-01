/* USER CODE BEGIN Header */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
//--------------------------------
#define REPORT                  0
//--------------------------------


#include "setVoltagesCurrents.h"
#if !REPORT
  #define ADC_CURRENT_LOAD        (unsigned int)0
  #define ADC_CURRENT_STANDBY     (unsigned int)1
  #define ADC_CURRENT_LOAD_REPEAT (unsigned int)2
  #define ADC_DT23                (unsigned int)3
  #define ADC_DT14                (unsigned int)4
  #define ADC_LENGTH_STR               10
#endif


#define RESET_VALUE                0x00
#define SLIGHT_WAIT_MS               10

#define TOTAL_CONNECTIONS             3
#define TOTAL_ADC_CH                  5
#define TOTAL_CAN_CONNECTIONS         2

#define CAN_FRAME_LEN                 5
#define LIN_FRAME_LEN                10


// Defines to all CANs
#define CAN_ID               (int)0x101
#define CAN_MSG                    0x01

// CAN1 Defines:
#define CAN1_UART_CAN_OK           0x01
#define CAN1_UART_CAN_ERROR        0x11

// CAN2 Defines:
#define CAN2_UART_CAN_OK           0x02
#define CAN2_UART_CAN_ERROR        0x22

// LIN Defines:
#define LIN_UART_MSG_RX            0x03
#define LIN_UART_MSG_OK            0x03
#define LIN_UART_MSG_ERROR         0x33

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

typedef struct
{
  uint32_t          canId;          // Id of CAN
  uint8_t           canMsg;         // Message of CAN

  uint8_t           uartRx;         // UART request message
  uint8_t           uartTxMsgOk;    // UART transmission, signalling that the CAN connection is ok
  uint8_t           uartTxMsgError; // UART transmission, signalling that the CAN connection is not ok

  bool              bIsConnected;   // True or False

  CAN_HandleTypeDef *pCan;          // CAN handle
}CanCommunication_t;


typedef struct
{
  uint8_t            uartRx;         // UART request message
  uint8_t            uartTxMsgOk;    // UART transmission, signalling that the LIN connection is ok
  uint8_t            uartTxMsgError; // UART transmission, signalling that the LIN connection is not ok
  bool               bIsConnected;   // True or False
  UART_HandleTypeDef *pUart;         // LIN handle
}LinCommunication_t;

#if !REPORT
typedef struct
{
  uint8_t           id;

  // Actual Values
  uint32_t          valueDecimal;   // For getting the real value
  float             valueVolts;     // For checking the value in volts
  float             valueCurrent;     // For checking the value in volts
  char              valueString[20]; // For sending the data using uart

  // Boundaries
  float             minValue;
  float             maxValue;
}AdcChannels_t;
#endif

typedef enum
{
  MainStates_GET_UART_CONNECTION_REQUESTS,
  MainStates_CHECK_CONNECTION_STATUS,
  MainStates_SEND_CONNECTION_STATUS
}MainStates_e;

typedef enum
{
  Connections_CAN,
  Connections_LIN
}Connections_e;


#include <userIncludes/can.h>
#include <userIncludes/lin.h>
#include <userIncludes/uart.h>
#include <userIncludes/inits.h>
#include <userIncludes/adc.h>


/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */
/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define LED1_Pin GPIO_PIN_1
#define LED1_GPIO_Port GPIOC
#define LED2_Pin GPIO_PIN_2
#define LED2_GPIO_Port GPIOC
#define LED3_Pin GPIO_PIN_3
#define LED3_GPIO_Port GPIOC
#define RELAY_12V_Pin GPIO_PIN_4
#define RELAY_12V_GPIO_Port GPIOA
#define ADC_CURRENT_SENSOR_Pin GPIO_PIN_5
#define ADC_CURRENT_SENSOR_GPIO_Port GPIOA
#define ADC_DT_14_Pin GPIO_PIN_5
#define ADC_DT_14_GPIO_Port GPIOC
#define ADC_DT_23_Pin GPIO_PIN_0
#define ADC_DT_23_GPIO_Port GPIOB
#define RELAY_DRIVETRAIN_Pin GPIO_PIN_1
#define RELAY_DRIVETRAIN_GPIO_Port GPIOB
#define RELAY_IGN_Pin GPIO_PIN_11
#define RELAY_IGN_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
