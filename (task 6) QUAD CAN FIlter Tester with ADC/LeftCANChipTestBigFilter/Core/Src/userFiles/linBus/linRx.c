/* linRx.c */
// Includes:
#include "userIncludes/lin.h"

// Global variables:
extern uint8_t gLinIRQMsg[LIN_FRAME_LEN];
extern uint8_t gLinChecksum;

//----------------
void HAL_UART_RxHalfCpltCallback(UART_HandleTypeDef *huart)
{
  if(huart -> Instance == USART2)
  {
    HAL_UART_Receive_DMA(huart, gLinIRQMsg, 3);
    gLinChecksum = gLinIRQMsg[2];
  }
}
