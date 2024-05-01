/*inits.h*/
#include "main.h"

C_Connections_t C_Init(uint8_t *name, C_Connections_t C_Connection);
C1_Connections_t C1_Init(CAN_HandleTypeDef *pCan, uint8_t *name, C1_Connections_t C1_Connection);


test1 C_AdcInit(uint8_t pinNum, test1 C_Adc);

