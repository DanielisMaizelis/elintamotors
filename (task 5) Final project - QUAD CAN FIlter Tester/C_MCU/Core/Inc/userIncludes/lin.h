/*
 * linBus.h
 *
 *  Created on: 7 Apr 2023
 *      Author: Maizel
 */
#include "main.h"

// lin folder:
// linTx.c
void linTx_sendMsg(uint8_t id, uint8_t *hUart);
uint8_t linTx_calcChecksum(uint8_t data[]);
