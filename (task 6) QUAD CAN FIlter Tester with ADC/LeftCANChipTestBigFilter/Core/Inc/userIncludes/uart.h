/*
 * uartRequests.h
 *
 *  Created on: 7 Apr 2023
 *      Author: Maizel
 */
#include "main.h"

// uartCommunication folder:
// uart.c
void uart_sendStatus(bool bConnectionOk, uint8_t msgConnected, uint8_t msgDisconnected, UART_HandleTypeDef *hUart);
bool uartRx_isLinOk(UART_HandleTypeDef *hUart2);
