/* canRx.c */
// Includes:
#include "main.h"

// Global variables:
extern uint8_t  gCanIRQReceivedMsg;

// Function Declarations:
bool canRx_isCanOk(uint8_t correctMsg);

//----------------
bool canRx_isCanOk(uint8_t correctMsg)
{
  if(gCanIRQReceivedMsg == correctMsg)
  {
    gCanIRQReceivedMsg = 0x00;
    return true;
  }
  else
  {
    gCanIRQReceivedMsg = 0x00;
    return false;
  }
}

// CAN1 reception:
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
  if(hcan->Instance == CAN1)
  {
    CAN_RxHeaderTypeDef RxHeader; // Struct specified
    uint8_t             canFrame[CAN_FRAME_LEN]; // Received 'hello' message

    // Gets the message
    if(HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &RxHeader, canFrame) != HAL_OK)
    {
      Error_Handler();
    }

    if(RxHeader.StdId == CAN_ID && RxHeader.RTR == 0) // Check the message details
    {
      gCanIRQReceivedMsg = canFrame[0];
    }
  }
}

// CAN2 reception:
void HAL_CAN_RxFifo1MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
  if(hcan->Instance == CAN2)
  {
    CAN_RxHeaderTypeDef RxHeader;                // Struct specified
    uint8_t             canFrame[CAN_FRAME_LEN]; // Received 'hello' message

    if(HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO1, &RxHeader, canFrame) != HAL_OK)// gets the message
    {
      Error_Handler();
    }

    if(RxHeader.StdId == CAN_ID && RxHeader.RTR == 0) // Check the message details
    {
      gCanIRQReceivedMsg = canFrame[0];
    }
  }
}
