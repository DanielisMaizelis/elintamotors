/* canFilterReception.c */
// Includes:
#include "userIncludes/can.h"

// Global Variables:
extern uint8_t gCanReceivedMsg;

// Function Declarations:
bool canRx_isCanOk(uint8_t correctMsg);

//----------------
// Check if the message from either CAN3 or CAN4 has been received:
bool canRx_isCanOk(uint8_t correctMsg)
{
  if(gCanReceivedMsg == correctMsg)
  {
    gCanReceivedMsg = RESET_VALUE;
    return true;
  }
  else
  {
    gCanReceivedMsg = RESET_VALUE;
    return false;
  }
}
// CAN1 reception:
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
  if(hcan->Instance == CAN1)
  {
    CAN_RxHeaderTypeDef RxHeader; // Struct specified
    uint8_t             canFrame[5]; // Received 'hello' message

    // Gets the message
    if(HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &RxHeader, canFrame) != HAL_OK)
    {
      Error_Handler();
    }

    if(RxHeader.StdId > 0x000 && RxHeader.RTR == 0) // Check the message details
    {
      gCanReceivedMsg = canFrame[0];
    }
  }
}

// CAN2 reception:
void HAL_CAN_RxFifo1MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
  if(hcan->Instance == CAN2)
  {
    CAN_RxHeaderTypeDef RxHeader;    // Struct specified
    uint8_t             canFrame[5]; // Received 'hello' message

    if(HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO1, &RxHeader, canFrame) != HAL_OK)// gets the message
    {
      Error_Handler();
    }

    if(RxHeader.StdId > 0x000 && RxHeader.RTR == 0) // Check the message details
    {
      gCanReceivedMsg = canFrame[0];
    }
  }
}


