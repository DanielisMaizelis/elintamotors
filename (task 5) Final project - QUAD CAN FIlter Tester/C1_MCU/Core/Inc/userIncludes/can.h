/* can.h */
#include "main.h"
// Folder: /can.c
void canConfig_filterConfig(CAN_HandleTypeDef *hCan);
bool canRx_isCanOk(uint8_t correctMsg);
void canTx_sendMsg(uint32_t canId, uint32_t canMsg, CAN_HandleTypeDef *hCan);
