/* uartTx.c */
// Includes:
#include "userIncludes/uart.h"

// Function Declarations:
void uart_sendStatus(bool bConnectionOk, uint8_t msgConnected, uint8_t msgDisconnected, UART_HandleTypeDef *hUart);

//----------------
// Sends connection status back in accordance to which connection was requested to the right MCU:
void uart_sendStatus(bool bIsConnected, uint8_t msgConnected, uint8_t msgDisconnected, UART_HandleTypeDef *hUart)
{
  uint8_t uartMsg;
  // Assigns the message sent using uart depending on the connection status
  if(bIsConnected)
  {
    uartMsg = msgConnected;
  }
  else
  {
    uartMsg = msgDisconnected;
  }

  // Sends the status message:
  HAL_UART_Transmit(hUart, &uartMsg, 1, 5);
}
