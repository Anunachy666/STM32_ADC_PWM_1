/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "system_def.h"

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define X8_33_MKP1_KEY1_PE2_Pin GPIO_PIN_2
#define X8_33_MKP1_KEY1_PE2_GPIO_Port GPIOE
#define X1_26_MKP3_RLED_PE4_Pin GPIO_PIN_4
#define X1_26_MKP3_RLED_PE4_GPIO_Port GPIOE
#define X1_24_MKP4_GLED_PE5_Pin GPIO_PIN_5
#define X1_24_MKP4_GLED_PE5_GPIO_Port GPIOE
#define X1_23_MKP5_BLED_PE6_Pin GPIO_PIN_6
#define X1_23_MKP5_BLED_PE6_GPIO_Port GPIOE
#define X8_35_MKP29_RET_KEY_PC3_Pin GPIO_PIN_3
#define X8_35_MKP29_RET_KEY_PC3_GPIO_Port GPIOC
#define X8_32_MKP34_TESTx_PA0_Pin GPIO_PIN_0
#define X8_32_MKP34_TESTx_PA0_GPIO_Port GPIOA
#define X8_32_MKP37_TESTA_PA3_Pin GPIO_PIN_3
#define X8_32_MKP37_TESTA_PA3_GPIO_Port GPIOA
#define X8_31_MKP41_ADC1_IN5_Pin GPIO_PIN_5
#define X8_31_MKP41_ADC1_IN5_GPIO_Port GPIOA
#define X8_15_MKP60_TIM1_CH1_Pin GPIO_PIN_9
#define X8_15_MKP60_TIM1_CH1_GPIO_Port GPIOE
#define X8_22_MKP64_TESTE_PE11_Pin GPIO_PIN_11
#define X8_22_MKP64_TESTE_PE11_GPIO_Port GPIOE
#define X1_18_MKP122_TEST1_PD6_Pin GPIO_PIN_6
#define X1_18_MKP122_TEST1_PD6_GPIO_Port GPIOD
#define X1_16_MKP126_TEST2_PG11_Pin GPIO_PIN_11
#define X1_16_MKP126_TEST2_PG11_GPIO_Port GPIOG
#define X1_14_MKP127_TEST3_PG12_Pin GPIO_PIN_12
#define X1_14_MKP127_TEST3_PG12_GPIO_Port GPIOG
#define X1_13_MKP132_TEST0_PG15_Pin GPIO_PIN_15
#define X1_13_MKP132_TEST0_PG15_GPIO_Port GPIOG

/* USER CODE BEGIN Private defines */
//******************************************************************************
/*
ТЗ:
  Необходимо реализовать чтение значений со входа АЦП с использованием DMA 
  с дискретизацией 10 КГц и установки полученных усреднённых значений на выходе
  PWM раз в 10млСек.
  А так же выдаче усреднённых за 1 секунду значений по интерфейсу UART.
  Полученные значения выводить в мили вольтах, формат представления в ASCII кодах
  с символом перевода строки на конце (пример: «1500 mV\r»).
  Дублировать вывод информации по SWD интерфейсу в терминал IDE Keil.

  Выбор ножек на усмотрение разработчика.

  Проект необходимо разработать в среде Keil,
  код должен быть читаем и описан в комментариях.

РЕАЛИЗАЦИЯ:
  Пины выбираются любые: АЦП=X8_32-MKP37-ADC1_IN3, ШИМ=X8_15-MKP60-TIM1_CH1
  Параметры АЦП выбираются следующие: разрядность 12 бит,
  частота > 10 кГц * 15 циклов, выбираем прескалер = 8 (APB=84MHz/8=10.5MHz=10500kHz)
  Выбираем запуск от таймера 2 с частотой событий 10 кГц
   (10500/15=68.3333333333333 > 10)
  Для передачи в память используем DMA2 Stream 0
  Запуск DMA осуществляем по программному таймеру через каждые 10 мс.
  Количество = 100 отсчётов.

  В связи с тем, что по ТЗ количество усреднёёных значений достаточно велико,
  можно использовать оверсэмплинг АЦП. Согласно теории "виртуальная разрядность АЦП"
  может быть увеличена на некоторое количество бит k, если количество усреднённых
  измерений пропорционально квадрату добавляемой точности. Т.е. количество измерений n
  должно быть не меньше n = (1 << k) ^^ 2. В нашем случае ближайшее число измерений
  в кадре, разлагаемое на квадраты 64 = (8) ^^ 2 = (1 << 3) ^^ 2. Поэтому можем
  поднять "виртуальную точность АЦП" на три бита, т.е. конечное среднее должно быть выше
  каждого максимального значения в 8 раз! Соответственно есть смысл и повысить разрешение
  вывода ШИМ до 15 бит, что соответствует периоду 32768. Однако уменьшим эту величину
  на один бит для стабильного результата и останолвимся на 14. В этом случае полявляются
  два дополнительных бита и всего разрешение принимаем как 14 бит, т.е. 16384 значений
  (0:16383 или 0x0000:0x3FFF). При входно частоте таймера 168 МГц частота обновления ШИМ
  будет 168000000 / 16384 = 10253.90625 Гц. Т.е. за 10 мС таймер обновится больше 100! раз,
  что вполне приемлемо.
  
  Вывод показаний АЦП (усреднёёно за 100 отсчётов) можно также выдавать с увеличенным
  разрешением. Вывод показаний в мВ рассчитывается по формуле:
   Umv = Uref * (Value / ((1 << res_bits) - 1)), цена "деления" при этом будет примерно
   Ux = Uref  / ((1 << res_bits) - 1). В нашем случае опорным напряжение выбираем напряжение
  питания, чтобы можно было подать на вход АЦП через делитель в пределах Uvcc - Uvdd.
  Тогда цена деления будет около 3.3В / 32767 = 0.00010071108127079073 В или примерно 1 мВ.
  За большой точностью тут гнаться бессмысленно в силу схемотехнических и физических
  ограничений, поэтому будем считать эту точность достаточной с учётом использования округления.
  Вывод у нас будет в целых мВ.
  Следует иметь ввиду, что доверять показаниям АЦП можно в определённых пределах: не менее 10% от 0
   и не более 99% максимального значения. Нижний предео ообъясняется шумом квантования
   (слишком большие "ступени"), а верхний предел определёнтем, что даже при напряжении АЦП выше Uref
   показания останутся максимальными (с учётом погрешности АЦП последовательнового приближения SAR).

  Для PWM выбираем TIM1 CH1 с простейшим RC филдьтром с частотой среза 100 Гц и спадом 6 дБ/октаву
  100 Гц => тау = 1 / (2* pi * freq) = 1000000 / (2 * 3.141529 * 100) = 1591.5816 = 1592 мкс.
  Соответствено из того, что нашёл тау = R*C = 100 нФ * (15 кОм + 100 Ом) = 1600 мкс.
  При частоте около 10 кГц (разница в 100 раз, чуть менее 7 октав) затухание должгно быть болен 40 дБ.

*/

#define VERSION			(0x10000000UL)
#define VERSION_TXT		("1.0.0.0")

#define MS2US(x)	(x*1000UL)

#define CONV_FRQ_kHz		(10UL)
#define ADC_RES_BITS		(12)
#define CONV_CYCLE_COUNT	(15)
#define ADC_MIN_FRQ_Hz		(CONV_FRQ_kHz*CONV_CYCLE_COUNT)

#define CONV_FRAME_TIME_mS	(10ULL)
#define CONV_FRAME_COUNT	(MS2US(CONV_FRAME_TIME_mS) * CONV_FRQ_kHz / 1000)
#define CONV_FRAME_OVERS_POW	(2)
#define CONV_FRAME_OVS_MULT	(1 << CONV_FRAME_OVERS_POW)

#define CHANNEL_COUNT		(2)

#define ADC_UREF		(3300UL)
#define ADC_MAX			((1 << (ADC_RES_BITS + CONV_FRAME_OVERS_POW)) - 1)
#define ADC2MV(x)		(((x*ADC_UREF)+((ADC_MAX+1)>>1))/(1<<(ADC_RES_BITS+CONV_FRAME_OVERS_POW)))

#define OUT_VALUE_TIME_mS	(1000UL)
#define OUT_VALUE_COUNT		(OUT_VALUE_TIME_mS / CONV_FRAME_TIME_mS)

// Максимальное отклонение от среднеарифметического значения ADC выше которого
// полученное значени е можно признать недействительным (помеха/ошибка)
#define ADC_MAX_ERR_THD		(50)

#define PWM_TIM1_CHANNEL	(TIM_CHANNEL_1)
#define PWM_RES_BITS		(15)
#define PWM_RES_PERIOD		(1 << PWM_RES_BITS)

#define LED_BLINK_TIME_mS	(500)

//******************************************************************************
/* USER CODE BEGIN Private defines */
//------------------------------------------------------------------------------
#define TEST0_Pin X1_13_MKP132_TEST0_PG15_Pin
#define TEST0_GPIO_Port X1_13_MKP132_TEST0_PG15_GPIO_Port
#define TEST1_Pin X1_18_MKP122_TEST1_PD6_Pin
#define TEST1_GPIO_Port X1_18_MKP122_TEST1_PD6_GPIO_Port
#define TEST2_Pin X1_16_MKP126_TEST2_PG11_Pin
#define TEST2_GPIO_Port X1_16_MKP126_TEST2_PG11_GPIO_Port
#define TEST3_Pin X1_14_MKP127_TEST3_PG12_Pin
#define TEST3_GPIO_Port X1_14_MKP127_TEST3_PG12_GPIO_Port
#define TESTA_Pin X8_32_MKP37_TESTA_PA3_Pin
#define TESTA_GPIO_Port X8_32_MKP37_TESTA_PA3_GPIO_Port
#define TESTE_Pin X8_22_MKP64_TESTE_PE11_Pin
#define TESTE_GPIO_Port X8_22_MKP64_TESTE_PE11_GPIO_Port
#define TESTx_Pin 
#define TESTx_GPIO_Port 
#define KEY1_Pin GPIO_PIN_2
#define KEY1_GPIO_Port GPIOE

#define RLED_Pin X1_26_MKP3_RLED_PE4_Pin
#define RLED_GPIO_Port X1_26_MKP3_RLED_PE4_GPIO_Port
#define OLED_Pin 
#define OLED_GPIO_Port 
#define GLED_Pin X1_24_MKP4_GLED_PE5_Pin
#define GLED_GPIO_Port X1_24_MKP4_GLED_PE5_GPIO_Port
#define BLED_Pin X1_23_MKP5_BLED_PE6_Pin
#define BLED_GPIO_Port X1_23_MKP5_BLED_PE6_GPIO_Port

#define USB1_OTG_VBUS_Pin GPIO_PIN_12
#define USB1_OTG_VBUS_GPIO_Port GPIOH

//******************************************************************************
#define RLEDON()	HAL_GPIO_WritePin(RLED_GPIO_Port, RLED_Pin, GPIO_PIN_RESET)
#define RLEDOFF()	HAL_GPIO_WritePin(RLED_GPIO_Port, RLED_Pin, GPIO_PIN_SET)
#define RLEDTGL()	HAL_GPIO_TogglePin(RLED_GPIO_Port, RLED_Pin)
#define isRLED()	(!HAL_GPIO_ReadPin(RLED_GPIO_Port, RLED_Pin))
//------------------------------------------------------------------------------
#define GLEDON()	HAL_GPIO_WritePin(GLED_GPIO_Port, GLED_Pin, GPIO_PIN_RESET)
#define GLEDOFF()	HAL_GPIO_WritePin(GLED_GPIO_Port, GLED_Pin, GPIO_PIN_SET)
#define GLEDTGL()	HAL_GPIO_TogglePin(GLED_GPIO_Port, GLED_Pin)
#define isGLED()	(!HAL_GPIO_ReadPin(GLED_GPIO_Port, GLED_Pin))
//------------------------------------------------------------------------------
#define OLEDON()	HAL_GPIO_WritePin(OLED_GPIO_Port, OLED_Pin, GPIO_PIN_RESET)
#define OLEDOFF()	HAL_GPIO_WritePin(OLED_GPIO_Port, OLED_Pin, GPIO_PIN_SET)
#define OLEDTGL()	HAL_GPIO_TogglePin(OLED_GPIO_Port, OLED_Pin)
#define isOLED()	(!HAL_GPIO_ReadPin(OLED_GPIO_Port, OLED_Pin))
//------------------------------------------------------------------------------
#define BLEDON()	HAL_GPIO_WritePin(BLED_GPIO_Port, BLED_Pin, BPIO_PIN_RESET)  
#define BLEDOFF()	HAL_GPIO_WritePin(BLED_GPIO_Port, BLED_Pin, BPIO_PIN_SET)
#define BLEDTGL()	HAL_GPIO_TogglePin(BLED_GPIO_Port, BLED_Pin)               
#define isBLED()	(!HAL_GPIO_ReadPin(BLED_GPIO_Port, BLED_Pin))
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
#ifdef __DEBUG
//------------------------------------------------------------------------------
#define TEST0ON()	HAL_GPIO_WritePin(TEST0_GPIO_Port, TEST0_Pin, GPIO_PIN_SET)
#define TEST0OFF()	HAL_GPIO_WritePin(TEST0_GPIO_Port, TEST0_Pin, GPIO_PIN_RESET)
#define TEST0TGL()	HAL_GPIO_TogglePin(TEST0_GPIO_Port, TEST0_Pin)
#define isTEST0()	(1 == HAL_GPIO_ReadPin(TEST0_GPIO_Port, TEST0_Pin))
//------------------------------------------------------------------------------
#define TEST1ON()	HAL_GPIO_WritePin(TEST1_GPIO_Port, TEST1_Pin, GPIO_PIN_SET)
#define TEST1OFF()	HAL_GPIO_WritePin(TEST1_GPIO_Port, TEST1_Pin, GPIO_PIN_RESET)
#define TEST1TGL()	HAL_GPIO_TogglePin(TEST1_GPIO_Port, TEST1_Pin)
#define isTEST1()	(1 == HAL_GPIO_ReadPin(TEST1_GPIO_Port, TEST1_Pin))
//------------------------------------------------------------------------------
#define TEST2ON()	HAL_GPIO_WritePin(TEST2_GPIO_Port, TEST2_Pin, GPIO_PIN_SET)
#define TEST2OFF()	HAL_GPIO_WritePin(TEST2_GPIO_Port, TEST2_Pin, GPIO_PIN_RESET)
#define TEST2TGL()	HAL_GPIO_TogglePin(TEST2_GPIO_Port, TEST2_Pin)
#define isTEST2()	(1 == HAL_GPIO_ReadPin(TEST2_GPIO_Port, TEST2_Pin))
//------------------------------------------------------------------------------
#define TEST3ON()	HAL_GPIO_WritePin(TEST3_GPIO_Port, TEST3_Pin, GPIO_PIN_SET)
#define TEST3OFF()	HAL_GPIO_WritePin(TEST3_GPIO_Port, TEST3_Pin, GPIO_PIN_RESET)
#define TEST3TGL()	HAL_GPIO_TogglePin(TEST3_GPIO_Port, TEST3_Pin)
#define isTEST3()	(1 == HAL_GPIO_ReadPin(TEST3_GPIO_Port, TEST3_Pin))
//------------------------------------------------------------------------------
#define TEST4ON()	HAL_GPIO_WritePin(TEST4_GPIO_Port, TEST4_Pin, GPIO_PIN_SET)
#define TEST4OFF()	HAL_GPIO_WritePin(TEST4_GPIO_Port, TEST4_Pin, GPIO_PIN_RESET)
#define TEST4TGL()	HAL_GPIO_TogglePin(TEST4_GPIO_Port, TEST4_Pin)
#define isTEST4()	(1 == HAL_GPIO_ReadPin(TEST4_GPIO_Port, TEST4_Pin))
//------------------------------------------------------------------------------
#define TESTAON()	HAL_GPIO_WritePin(TESTA_GPIO_Port, TESTA_Pin, GPIO_PIN_SET)
#define TESTAOFF()	HAL_GPIO_WritePin(TESTA_GPIO_Port, TESTA_Pin, GPIO_PIN_RESET)
#define TESTATGL()	HAL_GPIO_TogglePin(TESTA_GPIO_Port, TESTA_Pin)
#define isTESTA()	(1 == HAL_GPIO_ReadPin(TESTA_GPIO_Port, TESTA_Pin))
//------------------------------------------------------------------------------
#define TESTEON()	HAL_GPIO_WritePin(TESTE_GPIO_Port, TESTE_Pin, GPIO_PIN_SET)
#define TESTEOFF()	HAL_GPIO_WritePin(TESTE_GPIO_Port, TESTE_Pin, GPIO_PIN_RESET)
#define TESTETGL()	HAL_GPIO_TogglePin(TESTE_GPIO_Port, TESTE_Pin)
#define isTESTE()	(1 == HAL_GPIO_ReadPin(TESTE_GPIO_Port, TESTE_Pin))
//------------------------------------------------------------------------------
#define TESTxON()	HAL_GPIO_WritePin(TESTx_GPIO_Port, TESTx_Pin, GPIO_PIN_SET)
#define TESTxOFF()	HAL_GPIO_WritePin(TESTx_GPIO_Port, TESTx_Pin, GPIO_PIN_RESET)
#define TESTxTGL()	HAL_GPIO_TogglePin(TESTx_GPIO_Port, TESTx_Pin)
#define isTESTx()	(1 == HAL_GPIO_ReadPin(TESTx_GPIO_Port, TESTx_Pin))
//------------------------------------------------------------------------------
#else // __DEBUG
//------------------------------------------------------------------------------
#define TEST0ON()	
#define TEST0OFF()	
#define TEST0TGL()	
#define isTEST0()	(0)
//------------------------------------------------------------------------------
#define TEST1ON()	
#define TEST1OFF()	
#define TEST1TGL()	
#define isTEST1()	(0)
//------------------------------------------------------------------------------
#define TEST2ON()	
#define TEST2OFF()	
#define TEST2TGL()	
#define isTEST2()	(0)
//------------------------------------------------------------------------------
#define TEST3ON()	
#define TEST3OFF()	
#define TEST3TGL()	
#define isTEST3()	(0)
//------------------------------------------------------------------------------
#define TEST4ON()	
#define TEST4OFF()	
#define TEST4TGL()	
#define isTEST4()	(0)
//------------------------------------------------------------------------------
#define TESTAON()	
#define TESTAOFF()	
#define TESTATGL()	
#define isTESTA()	(0)
//------------------------------------------------------------------------------
#define TESTEON()	
#define TESTEOFF()	
#define TESTETGL()	
#define isTESTE()	(0)
//------------------------------------------------------------------------------
#define TESTxON()	
#define TESTxOFF()	
#define TESTxTGL()	
#define isTESTx()	(0)
//------------------------------------------------------------------------------
#endif // __DEBUG
//------------------------------------------------------------------------------
#define isKEY1()	(0 == HAL_GPIO_ReadPin(KEY1_GPIO_Port, KEY1_Pin))
//------------------------------------------------------------------------------

//******************************************************************************
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
