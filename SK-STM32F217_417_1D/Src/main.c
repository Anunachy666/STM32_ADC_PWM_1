/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "dma.h"
#include "iwdg.h"
#include "tim.h"
#include "usart.h"
#include "usb_device.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "usbd_cdc_if.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
volatile uint32_t AdcBuffConvCntr = 0;
static uint16_t AdcDblBuff[2][CONV_FRAME_COUNT*CHANNEL_COUNT];
static TickType_t AdcFrameNextTime = 0;
static uint32_t AdcLastBuffSumm = 0;
static uint32_t AdcLastRefSumm = 0;

static bool fAdcReady = true, fAdcDataReady = false;

static uint32_t OutAcc = 0;
static uint32_t OutRefAcc = 0;

static char OutTextBuf[128];
static TickType_t OutDataNextTime = 0;

static TickType_t LedBlinkNextTime = 0;

extern volatile TickType_t Tim1mS;
extern DMA_HandleTypeDef hdma_adc1;
extern TIM_HandleTypeDef htim2;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
//******************************************************************************
// User functions
//------------------------------------------------------------------------------
// For debug printf
int fputc(int ch, FILE *f)
{
  UNUSED(f);

  ITM_SendChar(ch);
  return(ch);
}
//------------------------------------------------------------------------------
__weak TickType_t Get1mSTickCount64(void)
{
  TickType_t res;
  __disable_interrupt();
  res = Tim1mS;
  __enable_interrupt();
  return res;
}
//------------------------------------------------------------------------------
__weak void sDelay(TickType_t Delay)
{
  Delay += Get1mSTickCount64();

  while(Get1mSTickCount64() > Delay) {
    __watchdog_reset();
  }
}

//******************************************************************************
//------------------------------------------------------------------------------
// Interrupt DMA Transfer Complete callback
static void DMA_AdcFrameCplt(DMA_HandleTypeDef *_hdma)
{
  if(&hdma_adc1 == _hdma) {	
    TEST1ON();
    AdcBuffConvCntr++;
    fAdcReady = true;
    HAL_ADC_Stop_DMA(&hadc1);
    TEST1OFF();
  }
}
//------------------------------------------------------------------------------
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(htim);

  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_TIM_PeriodElapsedCallback could be implemented in the user file
   */
  if(&htim1 == htim) {
    TEST3ON();
    TEST3OFF();
  }
  if(&htim2 == htim) {
    TEST2ON();
    TEST2OFF();
  }
}
//------------------------------------------------------------------------------
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hadc);
  /* NOTE : This function Should not be modified, when the callback is needed,
            the HAL_ADC_ConvCpltCallback could be implemented in the user file
  */
  if(&hadc1 == hadc) {
    TEST3ON();
    AdcBuffConvCntr++;
    fAdcReady = true;
    HAL_ADC_Stop_DMA(&hadc1);
    TEST3OFF();
  }
}
//------------------------------------------------------------------------------

static int AdcBuffSumm(const uint16_t *Buff, int Count, uint16_t ErrTreshold, uint32_t *Summ, uint32_t *SummRef)
{
  int ret = 0, Avg;

  *SummRef = *Summ = 0;

  Avg = (Buff[0] + Buff[CHANNEL_COUNT] + 1) >> 1;
  if(ErrTreshold > abs((int)Avg - (int)Buff[0]))
    *Summ = Buff[0];
  else {
    *Summ = Avg; ret--;
  }
  Avg = (Buff[1] + Buff[1+CHANNEL_COUNT] + 1) >> 1;
  if(ErrTreshold > abs((int)Avg - (int)Buff[1]))
    *SummRef = Buff[1];
  else {
    *SummRef = Avg; ret--;
  }

  for(int i=1;i<Count-1;i++)
  {
    
    Avg = (Buff[(i-1)*CHANNEL_COUNT] + Buff[(i+1)*CHANNEL_COUNT] + 1) >> 1;
    if(ErrTreshold > abs((int)Avg - (int)Buff[i*CHANNEL_COUNT]))
      *Summ += Buff[i*CHANNEL_COUNT];
    else {
      *Summ += Avg; ret--;
    }
    Avg = (Buff[(i-1)*CHANNEL_COUNT+1] + Buff[(i+1)*CHANNEL_COUNT+1] + 1) >> 1;
    if(ErrTreshold > abs((int)Avg - (int)Buff[i*CHANNEL_COUNT+1]))
      *SummRef += Buff[i*CHANNEL_COUNT+1];
    else {
      *SummRef += Avg; ret--;
    }
  }

  Avg = (Buff[(Count-2)*CHANNEL_COUNT] + Buff[(Count-1)*CHANNEL_COUNT] + 1) >> 1;
  if(ErrTreshold > abs((int)Avg - (int)Buff[(Count-1)*CHANNEL_COUNT]))
    *Summ += Buff[Count-1];
  else {
    *Summ = Avg; ret--;
  }
  Avg = (Buff[(Count-2)*CHANNEL_COUNT+1] + Buff[(Count-1)*CHANNEL_COUNT+1] + 1) >> 1;
  if(ErrTreshold > abs((int)Avg - (int)Buff[(Count-1)*CHANNEL_COUNT+1]))
    *SummRef += Buff[Count-1];
  else {
    *SummRef = Avg; ret--;
  }

  // Умножаем сумму на коэффициент оверсэмплинга 
  // K = (1<<CONV_FRAME_OVERS_POW)=(1<<2)=4)
  *Summ <<= CONV_FRAME_OVERS_POW; // Oversampling multipler
  *SummRef <<= CONV_FRAME_OVERS_POW; // Oversampling multipler

  return ret;
}
//------------------------------------------------------------------------------
static void SetPwmOut(uint32_t Value) {
  // Устанавливаем новое значение ШИМ
  TIM_OC_InitTypeDef pwmConfigOC;
  pwmConfigOC.OCMode = TIM_OCMODE_PWM1;
  pwmConfigOC.Pulse = Value;
  pwmConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  pwmConfigOC.OCNPolarity = TIM_OCNPOLARITY_HIGH;
  pwmConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  pwmConfigOC.OCIdleState = TIM_OCIDLESTATE_RESET;
  pwmConfigOC.OCNIdleState = TIM_OCNIDLESTATE_RESET;
  if (HAL_TIM_PWM_ConfigChannel(&htim1, &pwmConfigOC, PWM_TIM1_CHANNEL) != HAL_OK)
  {
    LOG_ERROR("%s :: HAL_TIM_PWM_ConfigChannel - fail\r\n", __func__);
    Error_Handler();
  }
  // Hack! For decrease setup time (two wariants))
  /* Set the Capture Compare Register value
  htim1.Instance->CCR1 = Value; */
  /* Set the Capture Compare Register value
  __HAL_TIM_SetCompare(&htim1, PWM_TIM1_CHANNEL, Value); */

}
//------------------------------------------------------------------------------
static HAL_StatusTypeDef OutTextVal(int AdcOutAvg, int AdcVrefAvg) {
  HAL_StatusTypeDef status;

  sprintf(OutTextBuf, "AdcVin = %d mV\r\n", (int)ADC2MV(AdcOutAvg));

  status = HAL_UART_Transmit_DMA(&huart1, (uint8_t*)OutTextBuf, strlen(OutTextBuf));
  status |= CDC_Transmit_FS((uint8_t*)OutTextBuf, strlen(OutTextBuf));
  // 13 uS
  LOG_PRINTF("AdcVin = %d mV, AdcVref = %d\r\n", (int)ADC2MV(AdcOutAvg), (int)ADC2MV(AdcVrefAvg));

  return status;
}

//******************************************************************************
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */
  HAL_StatusTypeDef status;
  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */
  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_IWDG_Init();
  MX_USART1_UART_Init();
  MX_USB_DEVICE_Init();
  MX_ADC1_Init();
  MX_TIM1_Init();
  MX_TIM2_Init();
  /* USER CODE BEGIN 2 */
  if (HAL_DMA_RegisterCallback(&hdma_adc1, HAL_DMA_XFER_CPLT_CB_ID, DMA_AdcFrameCplt) != HAL_OK)
  {
    Error_Handler();
  }

  memset(AdcDblBuff, 0, sizeof(AdcDblBuff));

  HAL_ADC_Stop_DMA(&hadc1);
  HAL_ADC_Stop(&hadc1);

  HAL_TIM_Base_Start_IT(&htim1);
  HAL_TIM_Base_Start_IT(&htim2);

  //__HAL_RCC_GPIOA_RELEASE_RESET();

  TESTAOFF();
  TESTEOFF();
  
  //SetPwmOut(1000);
  status = HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
  __HAL_TIM_SetCompare(&htim1, PWM_TIM1_CHANNEL, 1000);
  status = HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
  status = HAL_TIM_OC_Start(&htim1, TIM_CHANNEL_2);

  Sleep(150);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    __watchdog_reset();

    
    if((fAdcReady)&&(Get1mSTickCount() >= AdcFrameNextTime))
    {
      TEST1ON();
      AdcFrameNextTime = Get1mSTickCount() + CONV_FRAME_TIME_mS;

      fAdcReady = false;

      fAdcDataReady = true;
     
      status = HAL_ADC_Start_DMA(&hadc1, (uint32_t*)AdcDblBuff[AdcBuffConvCntr & 0x1],
                                 CONV_FRAME_COUNT*CHANNEL_COUNT);
      if(HAL_OK != status) {
      	LOG_ERROR("%s :: HAL_ADC_Start_DMA -fail, status = %d\r\n", __func__, (int)status);
      }
      TEST1OFF();
    }
    else 
    {
      if((Get1mSTickCount() >= OutDataNextTime))
      { // 29 uS
        OutDataNextTime = Get1mSTickCount() + OUT_VALUE_TIME_mS;

        int AdcOutAvg = (OutAcc + (OUT_VALUE_COUNT >> 1) + 1) / (OUT_VALUE_COUNT);
        int AdcRefAvg = (OutRefAcc + (OUT_VALUE_COUNT >> 1) + 1) / (OUT_VALUE_COUNT);
        OutRefAcc = OutAcc = 0;

        OutTextVal(AdcOutAvg, AdcRefAvg);
        GLEDTGL();
      }
    }

    if(fAdcDataReady)
    {
      fAdcDataReady = false;
      // Обработика полученного буфера
      TESTATGL();

      // Рассчитываем полную сумму по буферу
      AdcBuffSumm(AdcDblBuff[(AdcBuffConvCntr & 0x1) ^ 0x1], CONV_FRAME_COUNT,
                             ADC_MAX_ERR_THD, &AdcLastBuffSumm, &AdcLastRefSumm);
      // Получаем среднеарифметическое по буферу, для округления добавляенм половину количества
      uint32_t ValueAvg = (AdcLastBuffSumm + (CONV_FRAME_COUNT >> 1) + 1) / CONV_FRAME_COUNT;
      uint32_t ValueRefAvg = (AdcLastRefSumm + (CONV_FRAME_COUNT >> 1) + 1) / CONV_FRAME_COUNT;

      OutAcc += ValueAvg;
      OutRefAcc += ValueRefAvg;

      // Устанавливаем новое значение ШИМ
      SetPwmOut(ValueAvg);
      TESTETGL();
    }


    if(Get1mSTickCount() >= LedBlinkNextTime)
    {
      LedBlinkNextTime = Get1mSTickCount() + LED_BLINK_TIME_mS;
      RLEDTGL();
    }
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSI|RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 25;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
  HAL_RCC_MCOConfig(RCC_MCO1, RCC_MCO1SOURCE_HSE, RCC_MCODIV_1);
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}
#ifdef USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  UNUSED(file);
  UNUSED(line);
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
