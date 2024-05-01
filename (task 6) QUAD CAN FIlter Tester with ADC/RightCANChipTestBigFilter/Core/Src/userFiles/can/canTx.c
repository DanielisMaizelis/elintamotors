/* canFilterTransmission.c */
// Includes:
#include "userIncludes/can.h"
// Function Declarations:
void canTx_sendMsg(uint32_t canId, uint32_t canMsg, CAN_HandleTypeDef *hCan);


//----------------
void canTx_sendMsg(uint32_t canId, uint32_t canMsg, CAN_HandleTypeDef *hCan)
{
  uint8_t             messageToSend[1];
  CAN_TxHeaderTypeDef TxHeader;
  uint32_t            TxMailbox;

  TxHeader.DLC     = 1;                 // Length of the message in bytes - 'hello' is 5 bytes
  TxHeader.StdId   = canId;
  TxHeader.IDE     = CAN_ID_STD;        // Standard or extended
  TxHeader.RTR     = CAN_RTR_DATA;      // Data frame (request frame) or remote frame
  messageToSend[0] = canMsg;   // Message That I will Send

  // Tiny delay for making the transmission
  HAL_Delay(10);
  HAL_CAN_AddTxMessage(hCan, &TxHeader, messageToSend, &TxMailbox);
}
