/* canTx.c */
// Includes:
#include "main.h"

// Function Declarations:
void canTx_sendMsg(uint32_t canId, uint8_t canMsg, CAN_HandleTypeDef *hCan);

//----------------
void canTx_sendMsg(uint32_t canId, uint8_t canMsg, CAN_HandleTypeDef *hCan)
{
  uint8_t             sendMessage[1]; // Data sent
  uint32_t            TxMailbox;
  CAN_TxHeaderTypeDef TxHeader = {0};

  TxHeader.DLC   = 8;                 // Length of the message in bytes - 'hello' is 5 bytes
  TxHeader.StdId = canId;             // Standard Id
  TxHeader.IDE   = CAN_ID_STD;        // Standard or extended
  TxHeader.RTR   = CAN_RTR_DATA;      // Data frame (request frame) or remote frame

  sendMessage[0] = canMsg;            // Message That I will Send

  HAL_Delay(10);
  // Sends the message through the with the desired can details.
  HAL_CAN_AddTxMessage(hCan, &TxHeader, sendMessage, &TxMailbox);
}




