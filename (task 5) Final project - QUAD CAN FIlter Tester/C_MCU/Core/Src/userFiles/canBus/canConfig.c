/* canConfig.c */
// Includes:
#include "main.h"

// Function Declarations:
void canConfig_filterConfig(CAN_HandleTypeDef *hCan);
static void canConfig_startCans(CAN_HandleTypeDef *hCan, uint32_t fifoIrq);

//----------------
void canConfig_filterConfig(CAN_HandleTypeDef *hCan) // Configuring the CAN1 Filter
{

  CAN_FilterTypeDef canFilterInit = {0}; // Empty the memory
  uint32_t          fifoIrq;

  // Both CAN1 and CAN2 Configs
  canFilterInit.FilterActivation     = ENABLE;
  canFilterInit.FilterIdHigh         = 0x0000;
  canFilterInit.FilterIdLow          = 0x0000;
  canFilterInit.FilterMaskIdHigh     = 0x0000;
  canFilterInit.FilterMaskIdLow      = 0x0000;
  canFilterInit.FilterMode           = CAN_FILTERMODE_IDMASK;
  canFilterInit.FilterScale          = CAN_FILTERSCALE_32BIT;

  // CAN 1 Filter Configs
  if(hCan -> Instance == CAN1)
  {
    canFilterInit.FilterBank           = 1;
    canFilterInit.SlaveStartFilterBank = 14; // 14 each banks
    canFilterInit.FilterFIFOAssignment = CAN_RX_FIFO0;
    fifoIrq                            = CAN_IT_RX_FIFO0_MSG_PENDING;
  }

  // CAN 2 Filter Configs
  else if(hCan -> Instance == CAN2)
  {
    canFilterInit.FilterBank           = 15;
    canFilterInit.SlaveStartFilterBank = 14; // 14 each banks
    canFilterInit.FilterFIFOAssignment = CAN_RX_FIFO1;
    fifoIrq                            = CAN_IT_RX_FIFO1_MSG_PENDING;
  }

  if(HAL_CAN_ConfigFilter(hCan, &canFilterInit) != HAL_OK)
  {
    Error_Handler();
  }

  canConfig_startCans(hCan, fifoIrq);
}

static void canConfig_startCans(CAN_HandleTypeDef *hCan, uint32_t fifoIrq)
{
  if(HAL_CAN_ActivateNotification(hCan, CAN_IT_TX_MAILBOX_EMPTY | fifoIrq) != HAL_OK)
  {
    Error_Handler();
  }
  if(HAL_CAN_Start(hCan) != HAL_OK)
  {
    Error_Handler();
  }
}
