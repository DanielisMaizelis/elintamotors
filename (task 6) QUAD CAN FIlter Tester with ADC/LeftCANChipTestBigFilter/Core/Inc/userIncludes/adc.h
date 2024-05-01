/* currentSensor.h*/
#include "main.h"

AdcChannels_t inits_adcChannelInit(uint8_t channelId, AdcChannels_t adcChannel);
bool adc_sendAdcData(AdcChannels_t adcChannel[5], UART_HandleTypeDef *hUart1, ADC_HandleTypeDef *hAdc1, uint8_t testStage);
