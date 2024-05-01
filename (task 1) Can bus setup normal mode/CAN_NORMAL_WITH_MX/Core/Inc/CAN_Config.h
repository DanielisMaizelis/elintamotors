/*
 * CAN_Config.h
 *
 *  Created on: Jan 20, 2023
 *      Author: Maizel
 */

#ifndef INC_CAN_CONFIG_H_
#define INC_CAN_CONFIG_H_


#include "main.h"

// CAN1 Configurations
void CAN1_Tx(void);
void CAN1_Filter_Config(void);
void CAN1_IRQ_Start(void);

// CAN2 Configurations
void CAN2_Filter_Config(void);
void CAN2_IRQ_Start(void);

#endif /* INC_CAN_CONFIG_H_ */
