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
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct
{
  uint8_t             *name;
  uint8_t             uartMsg;
  uint8_t             uartMsgError;
  bool                bIsConnected;
} C_Connections_t;

// Connection test:
typedef struct
{
  uint8_t             *name;
  uint32_t            canId;
  uint8_t             canMsg;

  bool                bIsConnected;
  CAN_HandleTypeDef   *pCan;
} C1_Connections_t;

// Adc test:
typedef struct
{
  uint8_t            name[15];
  uint8_t            valueStr[10];
  bool               bCurrentOk;
}test1;

// Main states:
typedef enum
{
  MainStates_CHECK_CONNECTIONS,
  MainStates_PRINT_STATUS_USB
} MainStates_e;

// MCU test states:
typedef enum
{
  ModuleTested_CHECK_C_UART,
  ModuleTested_CHECK_C1,
} ModuleTested_e;

// Each MCU test stage:
typedef enum
{
  TestStages_TRANSMIT_MSG,
  TestStages_RECEIVE,
} TestStages_e;



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
#define PASS_C1_Pin GPIO_PIN_6
#define PASS_C1_GPIO_Port GPIOA
#define FAIL_C1_Pin GPIO_PIN_7
#define FAIL_C1_GPIO_Port GPIOA

/* USER CODE BEGIN Private defines */
  //---------------------------------------
  #define REPORT 0
  //---------------------------------------

  #define TOTAL_CONNECTIONS            5
  #define RESET_VALUE                  0x00

  // Time Defines
  #define UART_SYNC_DELAY_MS           10
  #define CONNECTION_CHECK_LIMIT       6000
  #define CONNECTION_DELAY             50

  // C1 Module Connections Details:
  #define CAN1_NAME                    "1. CAN1-U4"
  #define UART_CAN1_MSG                0x01
  #define UART_CAN1_MSG_ERROR          0x11

  #define CAN2_NAME                    "2. CAN2-U4"
  #define UART_CAN2_MSG                0x02
  #define UART_CAN2_MSG_ERROR          0x22

  #define LIN_NAME                     "3. LIN "
  #define UART_LIN_MSG                 0x03
  #define UART_LIN_MSG_ERROR           0x33

  // C Module Connections Details:
  #define CAN3_NAME                    "1. CAN3-U5"
  #define CAN3_ID                 (int)0x101
  #define CAN3_MSG                     0x01

  #define CAN4_NAME                    "2. CAN4-U6"
  #define CAN4_ID                 (int)0x101
  #define CAN4_MSG                     0x01

  #define ADC_DATA_LEN                    7
  #define TOTAL_ADC_CONN_C                3

  #include "userIncludes/adc.h"
  #include "userIncludes/uartRx.h"
  #include "userIncludes/can.h"
  #include "userIncludes/print.h"
  #include "userIncludes/inits.h"
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
