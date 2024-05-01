/*
 * canBus.h
 *
 *  Created on: 7 Apr 2023
 *      Author: Maizel
 */
#include "main.h"


// can folder:
// canConfig.c
void canConfig_filterConfig(CAN_HandleTypeDef *hCan);
// canTx.c
void canTx_sendMsg(uint32_t canId, uint8_t canMsg, CAN_HandleTypeDef *hCan);
// canRx.c
bool canRx_isCanOk(uint8_t correctMsg);
