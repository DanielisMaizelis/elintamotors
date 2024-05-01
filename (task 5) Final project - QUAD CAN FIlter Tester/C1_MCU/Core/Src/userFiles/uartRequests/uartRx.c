/* uartRx.c */
// Includes:
#include "userIncludes/uartRx.h"
// Global Variables:
extern UART_HandleTypeDef huart1;
extern uint8_t gUartByte;

// Function Declarations:
bool uartRx_isConnectionOk(uint8_t msgUart, uint8_t msgErrorUart);

//----------------
#if !REPORT
  void uartRx_receiveAdcDataC(UART_HandleTypeDef *hUart1);

  extern uint8_t gAdcRxDataUart[TOTAL_ADC_CONN_C][ADC_DATA_LEN];
  extern bool    gStartDmaUart;
  extern bool    gAllAdcReceived;
  extern uint8_t gChannelsTested;
  uint8_t digitCount = 0;
  uint8_t numberCount = 0;


  void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
  {
    if(huart -> Instance == USART1)
    {
     if(!gStartDmaUart)
     {
       if(digitCount == ADC_DATA_LEN)
       {
         if(gAdcRxDataUart[numberCount][0] == 'F')
         {
           gAllAdcReceived = true;
         }
         else if(gAdcRxDataUart[numberCount][0] == 'P')
         {
           numberCount++;
           digitCount = 0;
           if(numberCount == gChannelsTested)
           {
             gAllAdcReceived = true;
             gUartByte = 0;
             numberCount = 0;
           }
         }
       }
       else
       {
         gAdcRxDataUart[numberCount][digitCount] = gUartByte;
         digitCount++;
       }
       HAL_UART_Receive_IT(huart,&gUartByte, 1);
     }
   }
  }

  void uartRx_receiveAdcDataC(UART_HandleTypeDef *hUart1)
  {
    HAL_GPIO_WritePin(GPIOC, LED1_Pin|LED2_Pin, 1);
    while(!gAllAdcReceived)
    {
      HAL_UART_Receive_IT(hUart1,&gUartByte, 1);
    }
    gAllAdcReceived = false;
  }
#endif

// DMA UART Reception
void HAL_UART_RxHalfCallback(UART_HandleTypeDef *huart)
{
  if(huart -> Instance == USART1)
  {
    #if !REPORT
      if(gStartDmaUart)
      {
    #endif
      HAL_UART_Receive_DMA(huart,&gUartByte, 1);
    #if !REPORT
      }
    #endif
  }
}

// Receives the status of a requested message:
bool uartRx_isConnectionOk(uint8_t msgUart, uint8_t msgErrorUart)
{
  bool bWorking;
  while(1)
  {
    HAL_UART_Receive_DMA(&huart1,&gUartByte, 1);
    if(gUartByte == msgUart)
    {
      bWorking = true;
      break;
    }
    else if(gUartByte == msgErrorUart)
    {
      bWorking = false;
      break;
    }
  }
  return bWorking;
}






