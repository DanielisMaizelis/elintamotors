#include "userIncludes/inits.h"

C_Connections_t C_Init(uint8_t *name, C_Connections_t C_Connection);
C1_Connections_t C1_Init(CAN_HandleTypeDef *pCan, uint8_t *name, C1_Connections_t C1_Connection);
test1 C_AdcInit(uint8_t pinNum, test1 C_Adc);

C_Connections_t C_Init(uint8_t *name, C_Connections_t C_Connection)
{
  // CAN1 data:
  if(strcmp((char*)name,CAN1_NAME) == 0)
  {
    C_Connection.uartMsg      = UART_CAN1_MSG;
    C_Connection.uartMsgError = UART_CAN1_MSG_ERROR;
  }
  // CAN2 data:
  else if(strcmp((char*)name,CAN2_NAME) == 0)
  {
    C_Connection.uartMsg      = UART_CAN2_MSG;
    C_Connection.uartMsgError = UART_CAN2_MSG_ERROR;
  }
  // LIN data:
  else if(strcmp((char*)name,LIN_NAME) == 0)
  {
    C_Connection.uartMsg      = UART_LIN_MSG;
    C_Connection.uartMsgError = UART_LIN_MSG_ERROR;
  }
  C_Connection.name = name;
  return C_Connection;
}

C1_Connections_t C1_Init(CAN_HandleTypeDef *pCan, uint8_t *name, C1_Connections_t C1_Connection)
{
  // CAN 3 data:
  if(pCan -> Instance == CAN1)
  {
    C1_Connection.canId    = CAN3_ID;
    C1_Connection.canMsg   = CAN3_MSG;
  }
  // CAN4 data:
  else if(pCan -> Instance == CAN2)
  {
    C1_Connection.canId    = CAN4_ID;
    C1_Connection.canMsg   = CAN4_MSG;
  }
  C1_Connection.name     = name;
  C1_Connection.pCan     = pCan;
  canConfig_filterConfig(C1_Connection.pCan); // Filter Config for CAN3
  return C1_Connection;
}




