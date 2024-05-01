#include "CAN_Config.h"



/*
 * These are the Can 1 Configurations:
 */


extern TIM_HandleTypeDef htim6;
extern CAN_HandleTypeDef hcan1;
extern uint8_t message_can1_received;


void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim6) // When the period is elapsed
// It will activate the function.
{
	CAN1_Tx();
//	message_can1_received = 1;
}


// Step 1.
void CAN1_Tx(void) // First Message Transmission to CAN2
{
	CAN_TxHeaderTypeDef TxHeader;
	uint8_t send_message[5] = {'H','E','L','L','O'}; // Message That I will Send
	uint32_t TxMailbox; // Keeps the stored message and prioritises it

	TxHeader.DLC = 5; //Length of the message in bytes - 'hello' is 5 bytes
	TxHeader.StdId = 0x65D; // Standard 11bit identifier
	TxHeader.IDE = CAN_ID_STD; // Standard or extended
	TxHeader.RTR = CAN_RTR_DATA;  // Data frame (request frame) or remote frame

	if(HAL_CAN_AddTxMessage(&hcan1, &TxHeader, send_message, &TxMailbox) != HAL_OK)
	{ // If the message isn't sent into the mailbox
		Error_Handler();
		// Tells me the error of sending the data into mailbox
		// is not OK.
	}
}


// Step 4.
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
	CAN_RxHeaderTypeDef Rx_Header; // Struct specified
	uint8_t received_msg[5]; // Received 'hello' message

	if(HAL_CAN_GetRxMessage(&hcan1, CAN_RX_FIFO0, &Rx_Header, received_msg) != HAL_OK)// gets the message
	{
		Error_Handler();
	}

	if(Rx_Header.StdId == 0x65C && Rx_Header.RTR == 0) // Check the message details
	{
		message_can1_received = 1;
	}
}


//-------------------------------------------------------------------------------------------
// Configurations for CAN1


void CAN1_Filter_Config(void) // Configuring the CAN1 Filter
{
	CAN_FilterTypeDef can1_filter_init;
    can1_filter_init.FilterActivation = ENABLE;
    can1_filter_init.FilterBank = 1;
    can1_filter_init.SlaveStartFilterBank = 14; // 14 each banks
    can1_filter_init.FilterFIFOAssignment = CAN_RX_FIFO0;
    can1_filter_init.FilterIdHigh = 0x0000;
    can1_filter_init.FilterIdLow = 0x0000;
    can1_filter_init.FilterMaskIdHigh = 0x0000;
    can1_filter_init.FilterMaskIdLow = 0x0000;
    can1_filter_init.FilterMode = CAN_FILTERMODE_IDMASK;
    can1_filter_init.FilterScale = CAN_FILTERSCALE_32BIT;

    if(HAL_CAN_ConfigFilter(&hcan1, &can1_filter_init) != HAL_OK)
    {
    	Error_Handler();
    }
}



void CAN1_IRQ_Start(void) // This function is for starting the CAN1 and its IRQs
{
	if(HAL_CAN_ActivateNotification(&hcan1, CAN_IT_TX_MAILBOX_EMPTY | CAN_IT_RX_FIFO0_MSG_PENDING)!= HAL_OK)
	{
		Error_Handler();
	}
	if(HAL_CAN_Start(&hcan1) != HAL_OK)
	{
		Error_Handler();
	}
}



