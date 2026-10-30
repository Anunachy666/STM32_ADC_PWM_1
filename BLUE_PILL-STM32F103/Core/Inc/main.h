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
#include "stm32f1xx_hal.h"

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
#define PC13_OLED_MKP2_Pin GPIO_PIN_13
#define PC13_OLED_MKP2_GPIO_Port GPIOC
#define PA0_ADC1_IN0_MKP10_10_Pin GPIO_PIN_0
#define PA0_ADC1_IN0_MKP10_10_GPIO_Port GPIOA
#define PB12_TEST0_MKP25_Pin GPIO_PIN_12
#define PB12_TEST0_MKP25_GPIO_Port GPIOB
#define PB13_TEST1_MKP26_Pin GPIO_PIN_13
#define PB13_TEST1_MKP26_GPIO_Port GPIOB
#define PB14_TEST2_MKP27_Pin GPIO_PIN_14
#define PB14_TEST2_MKP27_GPIO_Port GPIOB
#define PB15_TEST3_MKP28_Pin GPIO_PIN_15
#define PB15_TEST3_MKP28_GPIO_Port GPIOB
#define PA8_TIM1_CH1_MKP29_Pin GPIO_PIN_8
#define PA8_TIM1_CH1_MKP29_GPIO_Port GPIOA
#define PB4_KEY1R_MKP40_Pin GPIO_PIN_4
#define PB4_KEY1R_MKP40_GPIO_Port GPIOB
#define PB5_KEY1_MKP41_Pin GPIO_PIN_5
#define PB5_KEY1_MKP41_GPIO_Port GPIOB
#define PB7_RLED_MKP43_Pin GPIO_PIN_7
#define PB7_RLED_MKP43_GPIO_Port GPIOB
#define PB8_GLED_MKP45_Pin GPIO_PIN_8
#define PB8_GLED_MKP45_GPIO_Port GPIOB
#define PB9_BLED_MKP46_Pin GPIO_PIN_9
#define PB9_BLED_MKP46_GPIO_Port GPIOB

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
  (0:16383 или 0x0000:0x3FFF). При входно частоте таймера 72 МГц частота обновления ШИМ
  будет 72000000 / 16384 = 4394.53125 Гц. Т.е. за 10 мС таймер обновится больше 40! раз,
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
  При частоте около 4 кГц (разница в 40 раз, чуть менее 6 октав) затухание должгно быть болен 32 дБ.

  Watch Dog Timer (WDT) - выбираем для тестового проекта (и при отладке) максимальное значение.
  Типично время WDT выбирается из расчёта (максимальная сумма всех задержек до сброса - детерминированное время)
  x (1 + 30%-50% = 1.5). Выбор времени сброса WDT зависит от прикладной задачи, время инициализации,
  времени максимальной задержки и времени восстановления нормальной работы. Таймер WDT используется только
  в конфигурации "Release", как "последний рубеж", когда по вине "объстоятельст непреодолимой силы" 
  МК мог зависнуть и его необходимо просто автономно перезапустить. Возражения типа 
  "хорошей программе ватчдог не нужен не принимаются"! Он защищает только от непридвиденных случайностей, 
  но не от программных "багов" (должны быть вычищены заранее).
  В настоящей конфигурации параметры WDT были такие: Prescaller=4, Timeout=4096(set 4096-1), что при
  тактовой частоте 40 кГц, соответствует максимальному периоду 
  1 / ((40000 / 4 / 4096) = 1 / 2.44140625 = 0.40960000000000002 c = 409 мС.
  Меняем на Prescaller=256, Timeout=4096(set 4096-1), получаем 1 / ((40000 / 4 / 4096) = 26.2144 С.,
  что вполне достаточно. Однако после инициализации WDT всё равно стоит произвести сброс WDN.

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
#define TEST0_Pin PB12_TEST0_MKP25_Pin
#define TEST0_GPIO_Port PB12_TEST0_MKP25_GPIO_Port
#define TEST1_Pin PB13_TEST1_MKP26_Pin
#define TEST1_GPIO_Port PB13_TEST1_MKP26_GPIO_Port
#define TEST2_Pin PB14_TEST2_MKP27_Pin
#define TEST2_GPIO_Port PB14_TEST2_MKP27_GPIO_Port
#define TEST3_Pin PB15_TEST3_MKP28_Pin
#define TEST3_GPIO_Port PB15_TEST3_MKP28_GPIO_Port
#define TESTA_Pin 
#define TESTA_GPIO_Port 
#define TESTE_Pin 
#define TESTE_GPIO_Port 
#define TESTx_Pin 
#define TESTx_GPIO_Port 
#define KEY1_Pin GPIO_PIN_2
#define KEY1_GPIO_Port GPIOE

#define RLED_Pin PB7_RLED_MKP43_Pin
#define RLED_GPIO_Port PB7_RLED_MKP43_GPIO_Port
#define OLED_Pin PC13_OLED_MKP2_Pin
#define OLED_GPIO_Port PC13_OLED_MKP2_GPIO_Port
#define GLED_Pin PB8_GLED_MKP45_Pin
#define GLED_GPIO_Port PB8_GLED_MKP45_GPIO_Port
#define BLED_Pin PB9_BLED_MKP46_Pin
#define BLED_GPIO_Port PB9_BLED_MKP46_GPIO_Port

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
