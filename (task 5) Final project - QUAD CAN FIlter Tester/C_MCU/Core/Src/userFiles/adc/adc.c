/* currentSensor.c */
// Includes:
#include "userIncludes/adc.h"

// Function Declarations:
static void adc_select(ADC_HandleTypeDef *hadcLeftMcu, uint8_t adcChannel);
bool adc_sendAdcData(AdcChannels_t adcChannel[5], UART_HandleTypeDef *hUart1, ADC_HandleTypeDef *hAdc1, uint8_t testStage);
static uint32_t adc_getDecimalValue(ADC_HandleTypeDef *hAdc1, uint8_t adcChannel);
static void adc_VoltageToString(float rawValue, char strValue[ADC_LENGTH_STR], int decimalNumCount);
AdcChannels_t inits_adcChannelInit(uint8_t channelId, AdcChannels_t adcChannel);
//----------------

AdcChannels_t inits_adcChannelInit(uint8_t channelId, AdcChannels_t adcChannel)
{
  if(channelId == ADC_CURRENT_LOAD)
  {
    adcChannel.minValue = CURRENT_LOAD_MIN;
    adcChannel.maxValue = CURRENT_LOAD_MAX;
  }
  else if(channelId == ADC_CURRENT_STANDBY)
  {
    adcChannel.minValue = -1;
    adcChannel.maxValue = 1;
  }
  else if(channelId == ADC_CURRENT_LOAD_REPEAT)
  {
    adcChannel.minValue = CURRENT_LOAD_AGAIN_MIN;
    adcChannel.maxValue = CURRENT_LOAD_AGAIN_MAX;
  }
  else if(channelId == ADC_DT23)
  {
    adcChannel.minValue = VOLTAGE_DT23_MIN;
    adcChannel.maxValue = VOLTAGE_DT23_MAX;
  }
  else if(channelId == ADC_DT14)
  {
    adcChannel.minValue = VOLTAGE_DT14_MIN;
    adcChannel.maxValue = VOLTAGE_DT14_MAX;
  }
  adcChannel.id = channelId;
  return adcChannel;
}

static void adc_select(ADC_HandleTypeDef *hAdc1, uint8_t adcChannel)
{
  ADC_ChannelConfTypeDef sConfig = {0};
  if(adcChannel == ADC_CURRENT_LOAD || adcChannel == ADC_CURRENT_STANDBY || adcChannel == ADC_CURRENT_LOAD_REPEAT)
  {
    sConfig.Channel = ADC_CHANNEL_5;
  }
  else if(adcChannel == ADC_DT23)
  {
    sConfig.Channel = ADC_CHANNEL_8;
  }
  else if(adcChannel == ADC_DT14)
  {
    sConfig.Channel = ADC_CHANNEL_15;
  }
  sConfig.Rank         = ADC_REGULAR_RANK_1;
  sConfig.SamplingTime = ADC_SAMPLETIME_239CYCLES_5;
  if(HAL_ADC_ConfigChannel(hAdc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
}

static uint32_t adc_getDecimalValue(ADC_HandleTypeDef *hAdc1, uint8_t adcChannel)
{
  uint32_t adcMeasuredVoltage;

  adc_select(hAdc1,adcChannel);
  HAL_ADC_Start(hAdc1);

  // Make a function for measuring these thingies:
  if(HAL_ADC_PollForConversion(hAdc1, 100) == HAL_OK)
  {
     // Gets the Value:
    adcMeasuredVoltage = HAL_ADC_GetValue(hAdc1);
    HAL_ADC_Stop(hAdc1);
  }
  return (uint32_t)adcMeasuredVoltage;
}

static void adc_VoltageToString(float rawValue, char strValue[ADC_LENGTH_STR], int decimalNumCount)
{
  float multiplier  = 1.0;
  int   digitNumLen = decimalNumCount+1;
  int   valueInt;
  // Ex: 1.234 is turned into "1234"
  // Ex: 0.234 is turned into "234"
  char  strDecimals[20] = {0};

  // Ex: 1234 is turned  into "1234"
  // Ex: 234 is turned   into "0234"
  char  strFull[ADC_LENGTH_STR] = {0};

  for(uint8_t i = 0;  i < decimalNumCount; i++)
  {
      multiplier*=10.0;
  }
  rawValue  *=  multiplier;
  valueInt   =  (int)rawValue;
  // Make the int value into a string:
  itoa(valueInt, strDecimals, 10);

  // Add zeros to front of the string, if the value is too small:
  if(digitNumLen-strlen(strDecimals) > 0)
  {
    for(uint8_t i = 0; i < digitNumLen-strlen(strDecimals); i++)
    {
      strcat(strFull,"0");
    }
  }

  // Combine the values of the integer string and the added zeros on the front:
  // If the value is too small.
  strcat(strFull,strDecimals);
  strValue[0] = strFull[0];
  strValue[1] = '.';

  for(int i=1; i<digitNumLen; i++)
  {
    strValue[i+1] = strFull[i];
  }
}


bool adc_sendAdcData(AdcChannels_t adcChannel[5], UART_HandleTypeDef *hUart1, ADC_HandleTypeDef *hAdc1, uint8_t testStage)
{
  uint8_t lastTestedConn;
  uint8_t totalConn;
  char    strTotalAdcValue[20] = {0};
  bool    bCurrentOk = true;

  if(testStage == 1)
  {
    lastTestedConn = 0;
    totalConn = 2;
  }
  else if(testStage == 2)
  {
    lastTestedConn = 2;
    totalConn = 3;
  }

  for(uint8_t chNum = lastTestedConn; chNum < totalConn+lastTestedConn; chNum++)
  {
    // memset
    memset(adcChannel[chNum].valueString, 0, sizeof(adcChannel[chNum].valueString));
    memset(strTotalAdcValue, 0, sizeof(strTotalAdcValue));

    if(chNum == 0)  // If it is a current sensor pin tested
    {
      HAL_Delay(500);
      HAL_GPIO_WritePin(GPIOA, RELAY_12V_Pin, 1); // Turns on a source voltage relay
    }
    else if(chNum == 1 && bCurrentOk)
    {
      HAL_GPIO_WritePin(GPIOB, RELAY_IGN_Pin, 1); // Turns On Ignition relay for quad CAN Filter
    }
    else if(chNum == 2 && bCurrentOk)
    {
      HAL_GPIO_WritePin(GPIOB, RELAY_DRIVETRAIN_Pin, 1); // Turns On Ignition relay for quad CAN Filter
    }

    // Gets adc value as an integer
    adcChannel[chNum].valueDecimal = adc_getDecimalValue(hAdc1, adcChannel[chNum].id);
    if(chNum == 0 || chNum == 2 || chNum == 1)
    {
      adcChannel[chNum].valueVolts = (3.3 - (((float) adcChannel[chNum].valueDecimal / (float) 4095) * 3.3))/0.5;
    }
    else
    {
      adcChannel[chNum].valueVolts = (((float) adcChannel[chNum].valueDecimal / (float) 4095) * 3.3)*(29.7/4.7);
    }
    if(adcChannel[chNum].valueVolts < 0)
    {
      adcChannel[chNum].valueVolts *= -1;
    }

    // If the currents from the adc channels are exceeded
    if(adcChannel[chNum].valueVolts < -1 || adcChannel[chNum].valueVolts > 20)
    {
      bCurrentOk = false;
      HAL_GPIO_WritePin(GPIOA, RELAY_12V_Pin, 0);
      HAL_GPIO_WritePin(GPIOB, RELAY_IGN_Pin, 0);
      HAL_GPIO_WritePin(GPIOB, RELAY_DRIVETRAIN_Pin, 0);
      strcat(adcChannel[chNum].valueString, "F");
    }
    else
    {
      strcat(adcChannel[chNum].valueString, "P");
    }

    // Converts a value into a string
    adc_VoltageToString(adcChannel[chNum].valueVolts, strTotalAdcValue, 5);
    strcat(adcChannel[chNum].valueString, strTotalAdcValue);

    HAL_UART_Transmit(hUart1, (uint8_t*)adcChannel[chNum].valueString, 8, 50);
    HAL_GPIO_TogglePin(GPIOC, LED1_Pin|LED2_Pin);

    if(!bCurrentOk)
    {
      return false;
    }
    HAL_Delay(10);
   }
  return true;
 }




