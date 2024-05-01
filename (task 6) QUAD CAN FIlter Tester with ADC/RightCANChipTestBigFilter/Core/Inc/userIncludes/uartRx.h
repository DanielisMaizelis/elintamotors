/*
 * uart.h
 *
 *  Created on: 8 Apr 2023
 *      Author: Maizel
 */
#include "main.h"
// Folder: /can1AndCan2_Uart
bool uartRx_isConnectionOk(uint8_t msgUart, uint8_t msgErrorUart);
void uartRx_receiveAdcDataC(UART_HandleTypeDef *hUart1);
