/*printResults.h */
#include "main.h"
// Folder: /print
void print_connectionStatus(uint8_t connectionNum, uint8_t *name, bool bIsConnectionOk, UART_HandleTypeDef *pUsbUart);

void print_startMenu(void);
void print_endNote(bool bTestPass);
void print_prData(char data[100]);


void print_adcDataC(uint8_t adcReceivedData[TOTAL_ADC_CONN_C][ADC_DATA_LEN], test1 C_Adc[6], uint8_t connAmount);
