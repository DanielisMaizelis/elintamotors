/* printResults.c */

// Includes:
#include "userIncludes/print.h"

extern UART_HandleTypeDef huart2;
extern uint8_t turnOn;

// FUNCTION DECLARATIONS ----------------

// Connection Status printing
void print_connectionStatus(uint8_t connectionNum, uint8_t *name, bool bIsConnectionOk, UART_HandleTypeDef *pUsbUart);

// Menu Items:
void print_startMenu(void);
void print_endNote(bool bTestPass);

// Standard printing:
void print_prData(char data[100]);

void print_adcDataC(uint8_t adcReceivedData[TOTAL_ADC_CONN_C][ADC_DATA_LEN], test1 C_Adc[6], uint8_t connAmount);

// ----------------

void print_prData(char data[100])
{
  HAL_UART_Transmit(&huart2, (uint8_t*)data, strlen(data), strlen(data)*2);
}

void print_startMenu(void)
{
  print_prData("---------------------\r\n");
  print_prData("QUAD CAN FILTER TESTER  \r\n\n");
  print_prData("Testing... \r\n");
}

void print_endNote(bool bTestPass)
{
  HAL_Delay(10);
  print_prData("\r\n\nTEST ");
  if(bTestPass)
  {
    print_prData("SUCCESS!");
    HAL_GPIO_WritePin(GPIOA,PASS_C1_Pin, 0);
    HAL_GPIO_WritePin(GPIOA,FAIL_C1_Pin, 1);
    HAL_GPIO_WritePin(GPIOC, LED1_Pin|LED2_Pin, 1);
  }
  else
  {
    print_prData("FAIL! (CONNECTIONS)");
    HAL_GPIO_WritePin(GPIOA,PASS_C1_Pin, 1);
    HAL_GPIO_WritePin(GPIOA,FAIL_C1_Pin, 0);
    HAL_GPIO_WritePin(GPIOC, LED1_Pin, 1);
  }
  HAL_Delay(5);
  print_prData("\r\n---------------------\r\n\n");
}

void print_connectionStatus(uint8_t connectionNum, uint8_t *name, bool bIsConnectionOk, UART_HandleTypeDef *pUsbUart)
{
   if(connectionNum == 0)
   {
     print_prData("\r\n-- C CONNECTIONS --\r\n");
   }
   else if(connectionNum == 3)
   {
     print_prData("\n-- C1 CONNECTIONS --\r\n");
   }
   print_prData((char*)name);
   HAL_Delay(10);
   print_prData(" Status: ");
   // Depending of the status of connection assign a string, which tells if the connection is good:
   if(bIsConnectionOk)
   {
     print_prData("OK");
   }
   else
   {
     print_prData("ERROR");
   }
   print_prData("\r\n");
}

void print_adcDataC(uint8_t adcReceivedData[TOTAL_ADC_CONN_C][ADC_DATA_LEN], test1 C_Adc[6], uint8_t connAmount)
{
    bool    bConStatus = true;
    uint8_t lastTestedConn;

    if(connAmount == 3)
    {
      lastTestedConn = 2;
    }
    else if(connAmount == 2)
    {
      lastTestedConn = 0;
    }

    for(int adcConnC = lastTestedConn; adcConnC<connAmount+lastTestedConn; adcConnC++) // Printing the adc currents:
    {
      memset(C_Adc[adcConnC].valueStr, 0, sizeof(C_Adc[adcConnC].valueStr)); // Clear the array using memset

      for(int charact=0; charact<ADC_DATA_LEN; charact++) // characters are listed using thois for loop
      {
        C_Adc[adcConnC].valueStr[charact] = (uint8_t)adcReceivedData[adcConnC-lastTestedConn][charact]; // Add new characters from an interrupt global variable
      }

      if(C_Adc[adcConnC].valueStr[0] == 'F')
      {
        bConStatus = false;
      }

      for(int charact=1; charact<ADC_DATA_LEN; charact++) // characters are listed using thois for loop
      {
        C_Adc[adcConnC].valueStr[charact-1] = (uint8_t)adcReceivedData[adcConnC-lastTestedConn][charact]; // Add new characters from an interrupt global variable
      }

      // Prints the explanation for each voltage data sent:
      switch(adcConnC)
      {
         case 0:
           HAL_Delay(5);
           print_prData("\r\n-- CURRENTS --\r\n");
           print_prData("1. Current Stand-by: ");
           print_prData((char*)C_Adc[adcConnC].valueStr);
           print_prData(" A");
           break;
         case 1:
           print_prData("2. Current Load:     ");
           print_prData((char*)C_Adc[adcConnC].valueStr);
           print_prData(" A");
           break;
         case 2:
           HAL_Delay(5);
           print_prData("\r\n-- CURRENTS --\r\n");
           print_prData("1. Current Load:     ");
           print_prData((char*)C_Adc[adcConnC].valueStr);
           print_prData(" A");
           break;
         case 3:
           HAL_Delay(5);
           print_prData("\r\n-- VOLTAGES--\r\n");
           print_prData("2. Drivetrain 23:    ");
           print_prData((char*)C_Adc[adcConnC].valueStr);
           print_prData(" V");
           break;
         case 4:
           HAL_Delay(5);
           print_prData("3. Drivetrain 14:    ");
           print_prData((char*)C_Adc[adcConnC].valueStr);
           print_prData(" V");
          break;
      }

      if(!bConStatus)
      {
        HAL_GPIO_WritePin(GPIOA,PASS_C1_Pin , 1);
        HAL_GPIO_WritePin(GPIOA,FAIL_C1_Pin , 0);
        HAL_Delay(10);
        print_prData("  <- FAIL!");
        HAL_Delay(20);
        print_prData("\r\n\nTEST ABORTED!");
        print_prData("\r\n---------------------\r\n\n");
        turnOn = 2;
        Error_Handler();
      }
      else
      {
        print_prData("  OK!\r\n");
      }
    }
  }



