/*inits.c*/
#include "userIncludes/inits.h"

CanCommunication_t inits_canInit(CAN_HandleTypeDef *hCan, CanCommunication_t can);
LinCommunication_t inits_linInit(LinCommunication_t lin);

CanCommunication_t inits_canInit(CAN_HandleTypeDef *hCan, CanCommunication_t can)
{
  if(hCan -> Instance == CAN1)
  {
    can.uartRx         = CAN1_UART_CAN_OK;
    can.uartTxMsgOk    = CAN1_UART_CAN_OK;
    can.uartTxMsgError = CAN1_UART_CAN_ERROR;
  }
  else if(hCan -> Instance == CAN2)
  {
    can.uartRx         = CAN2_UART_CAN_OK;
    can.uartTxMsgOk    = CAN2_UART_CAN_OK;
    can.uartTxMsgError = CAN2_UART_CAN_ERROR;
  }
  can.canId          = CAN_ID;
  can.canMsg         = CAN_MSG;
  can.pCan = hCan;
  canConfig_filterConfig(can.pCan);

  return can;
}

LinCommunication_t inits_linInit(LinCommunication_t lin)
{
  lin.uartRx         = LIN_UART_MSG_RX;
  lin.uartTxMsgOk    = LIN_UART_MSG_OK;
  lin.uartTxMsgError = LIN_UART_MSG_ERROR;

  return lin;
}

