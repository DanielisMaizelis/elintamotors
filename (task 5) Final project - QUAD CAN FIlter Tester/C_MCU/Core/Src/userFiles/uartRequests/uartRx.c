/* uart.c */
// Includes:
#include "userIncludes/uart.h"
extern uint8_t gLinChecksum;
extern uint8_t gLinIRQMsg[LIN_FRAME_LEN];


// Global Variables:
extern uint8_t gUartConnRequest;

// Gets requested connection from uart:
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
  // UART reception from Right Mcu
  if(huart -> Instance == USART1)
  {
    HAL_UART_Receive_DMA(huart, &gUartConnRequest, 1);
  }
}


bool uartRx_isLinOk(UART_HandleTypeDef *hUart2)
{
  bool bConnStatus = false;
  gLinChecksum = 0x00;
  // Tries to receive the sent message while the timer is ticking
  for(uint32_t i = 0; i < 100000; i++)
  {
    HAL_UART_Receive_DMA(hUart2, gLinIRQMsg, 4);
    if(gLinChecksum > 0x00)
    {
      bConnStatus = true;
      gLinChecksum = 0x00;
    }
  }
  return bConnStatus;
}
