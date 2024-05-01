#include "CAN_Config.h"



/*
 * These are the Can 2 Configurations:
 */
void CAN2_Tx(void);

extern CAN_HandleTypeDef hcan2;
extern uint8_t message_can2_received;

// Step 2.
void HAL_CAN_RxFifo1MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
	CAN_RxHeaderTypeDef Rx_Header; // Struct specified
	uint8_t received_msg[5]; // Received 'hello' message

	if(HAL_CAN_GetRxMessage(&hcan2, CAN_RX_FIFO1, &Rx_Header, received_msg) != HAL_OK)// gets the message
	{
		Error_Handler();
	}

	if(Rx_Header.StdId == 0x65D && Rx_Header.RTR == 0) // Check the message details
	{
		//--
		CAN2_Tx();
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4|GPIO_PIN_2|GPIO_PIN_7, 0);
		message_can2_received = 1;
	}
}

// Step 3.
void CAN2_Tx(void) // +
{
	CAN_TxHeaderTypeDef TxHeader;
	uint8_t send_message[5] = {'B','Y','E'}; // Message That I will Send
	uint32_t TxMailbox; // Keeps the stored message and prioritises it

	TxHeader.DLC = 3; //Length of the message in bytes - 'bye' is 3 bytes
	TxHeader.StdId = 0x65C; // Standard 11bit identifier
	TxHeader.IDE = CAN_ID_STD; // Standard or extended
	TxHeader.RTR = CAN_RTR_DATA;  // Data frame (request frame) or remote frame

	if(HAL_CAN_AddTxMessage(&hcan2, &TxHeader, send_message, &TxMailbox) != HAL_OK)
	{ // If the message isn't sent into the mailbox
		Error_Handler();
		// Tells me the error of sending the data into mailbox
		// is not OK.
	}
}

//-------------------------------------------------------------------------------------------
// Configurations for CAN2



void CAN2_Filter_Config(void)
{
	CAN_FilterTypeDef can2_filter_init = {0};
	can2_filter_init.FilterActivation = ENABLE;
	can2_filter_init.FilterBank = 15;
	can2_filter_init.SlaveStartFilterBank = 14; //14 each banks
	can2_filter_init.FilterFIFOAssignment = CAN_RX_FIFO1;
	can2_filter_init.FilterIdHigh = 0x0000;
	can2_filter_init.FilterIdLow = 0x0000;
	can2_filter_init.FilterMaskIdHigh = 0x0000;
	can2_filter_init.FilterMaskIdLow = 0x0000;
	can2_filter_init.FilterMode = CAN_FILTERMODE_IDMASK;
	can2_filter_init.FilterScale = CAN_FILTERSCALE_32BIT;

	if(HAL_CAN_ConfigFilter(&hcan2, &can2_filter_init) != HAL_OK)
	{
		Error_Handler();
	}
}


void CAN2_IRQ_Start(void)
{
	if(HAL_CAN_ActivateNotification(&hcan2, CAN_IT_RX_FIFO1_MSG_PENDING)!= HAL_OK)
	{
		Error_Handler();
	}
	if(HAL_CAN_Start(&hcan2) != HAL_OK)
	{
		Error_Handler();
	}
}


