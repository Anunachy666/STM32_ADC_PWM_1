//******************************************************************************
// system_def.h
//******************************************************************************
//
//	FULLNAME:  System definitions
//
//	NICKNAME:
//
//	PURPOSE:   System definitions
//
//	TOOLKIT:   EWAVR 8.11 (IAR Systems)
//
//	Version: 1.01
//
//	$Revision: 101 $
//	$Date:: 2017-08-18 #$
//
//	Copyright (c) 2017, Igor Ivanovich Zorin
//
//	Permission is hereby granted, free of charge, to any person obtaining  a copy of
//	this software and  associated documentation files  (the "Software"), to  deal in
//	the Software  without restriction,  including without  limitation the  rights to
//	use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
//	the Software, and to permit persons to whom the Software is furnished to do  so,
//	subject to the following conditions:
//
//	The above copyright notice and this permission notice shall be included
//	in all copies or substantial portions of the Software.
//
//	THE SOFTWARE  IS PROVIDED  "AS IS",  WITHOUT WARRANTY  OF ANY  KIND, EXPRESS  OR
//	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
//	FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE AUTHORS  OR
//	COPYRIGHT HOLDERS BE LIABLE FOR  ANY CLAIM, DAMAGES OR OTHER  LIABILITY, WHETHER
//	IN  AN  ACTION OF  CONTRACT,  TORT OR  OTHERWISE,  ARISING FROM,  OUT  OF OR  IN
//	CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//
//******************************************************************************

#ifndef SYSTEM_DEF_H
#define SYSTEM_DEF_H

//******************************************************************************
//#include <intrinsics.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
//------------------------------------------------------------------------------
#include <TYPE.h>
#include <Errors_1.h>
//------------------------------------------------------------------------------
//#include "FreeRTOS.h"
//#include "cmsis_os.h"
//#include "iwdg.h"
//#include "fmc.h"
//------------------------------------------------------------------------------
//#include "ff.h"
//------------------------------------------------------------------------------
//#include <ext_memory.h>
//------------------------------------------------------------------------------
//******************************************************************************

//******************************************************************************
#ifdef STM32F103xB
#include "stm32f1xx.h"
#include "stm32f1xx_hal.h"
#elif defined(STM32L151xBA)
#include "stm32l1xx.h"
#include "stm32l1xx_hal.h"
#elif defined(STM32F412Rx)
#include "stm32f4xx.h"
#include "stm32f4xx_hal.h"
#elif defined(STM32F415xx)
#include "stm32f4xx.h"
#include "stm32f4xx_hal.h"
#elif defined(STM32F417xx)
#include "stm32f4xx.h"
#include "stm32f4xx_hal.h"
#elif defined(STM32F429xx)
#include "stm32f4xx.h"
#include "stm32f4xx_hal.h"
#elif defined(STM32F437xx)
#include "stm32f4xx.h"
#include "stm32f4xx_hal.h"
#elif defined(STM32F746xx)
#include "stm32f7xx.h"
#include "stm32f7xx_hal.h"
#else
  #error "MCU type not defined!"
#endif
//------------------------------------------------------------------------------
//******************************************************************************
#ifndef TError
//typedef int	TError;  // тип результата 0 < Error - OK или полезное значение, 0 > Error - код ошибки
#endif
//------------------------------------------------------------------------------
typedef struct
{
  unsigned char		sec;
  unsigned char		min;
  unsigned char		hour;
  unsigned char		date;
  unsigned char		month;
  unsigned char		day;
  unsigned short	year;
} sDateTime;
//------------------------------------------------------------------------------
//******************************************************************************

//******************************************************************************
#define USE_FREERTOS    0
//------------------------------------------------------------------------------
//#define __DEBUG

#if __DEBUG
  #define LOG_PRINTF printf 
  #define LOG_MESSAGE printf
  #define LOG_WARNING printf
  #define LOG_ERROR printf

  #ifdef USB_TRACE_DEBUG
    #define TRACE_DEBUG		printf
    #define TRACE_INFO_WP	printf
  #else
    #define TRACE_DEBUG(...)
    #define TRACE_INFO_WP(...)
  #endif // USB_TRACE_DEBUG

#else // __DEBUG
  #define LOG_PRINTF(...)
  #define LOG_MESSAGE(...)
  #define LOG_WARNING(...)
  #define LOG_ERROR(...)

  #define TRACE_DEBUG(...)
  #define TRACE_INFO_WP(...)

#endif // __DEBUG

//------------------------------------------------------------------------------
//******************************************************************************

//******************************************************************************
#if USE_FREERTOS
//------------------------------------------------------------------------------
#endif // USE_FREERTOS
//******************************************************************************

//******************************************************************************
//------------------------------------------------------------------------------
#define	DEVICE_POWER	(1<<12)
#define	DEVICE_OPEN	(1<<13)
#define	DEVICE_INIT	(1<<14)
#define	DEVICE_BUSY	(1<<15)
#define	DEVICE_ERROR	(1<<31)
//------------------------------------------------------------------------------
//typedef int	TError;
//------------------------------------------------------------------------------
//******************************************************************************

//******************************************************************************
//------------------------------------------------------------------------------
typedef uint64_t	TickType_t;
//------------------------------------------------------------------------------
//extern uint64_t Get1mSTickCount(void);
#define	Get1mSTickCount	HAL_GetTick
//------------------------------------------------------------------------------
#define	__enable_interrupt	__enable_irq
#define	__disable_interrupt	__disable_irq
//------------------------------------------------------------------------------
//#define	Sleep	osDelay
#define	Sleep	HAL_Delay
//#define	Sleep	sDelay
//******************************************************************************

//******************************************************************************
extern IWDG_HandleTypeDef hiwdg;
//------------------------------------------------------------------------------
#define	__watchdog_reset()      HAL_IWDG_Refresh(&hiwdg);
//------------------------------------------------------------------------------
#define	xMalloc	malloc
#define	xFree	free
//#define	xMalloc	pvPortMalloc
//#define	xFree	vPortFree
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//******************************************************************************

//******************************************************************************
// Version info
//------------------------------------------------------------------------------
//#define	dModuleVersion		"TM1AA-001"
//------------------------------------------------------------------------------
//******************************************************************************

//******************************************************************************
typedef struct
{
  char	ModuleVersion[16];	// Версия прошивки
  char	HardwareVersion[16];	// Версия "железа"
  char	PCBVersion[16];		// Версия платы
  char	BodyVersion[16];	// Версия корпуса
  char	BootloaderVersion[16];	// Версия бутлоадера
  char	StrategyVersion[16];	// Версия стратегии
  char	CalibrationVersion[16];	// Версия калибровки
  char	CalibrVersion[16];	// Версия калибровок (настройки под машину)

  char	BuildDateTime[32];	// Дата и Вреям текущего билда прошивки
}TStrVersions;
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//******************************************************************************

//******************************************************************************
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//******************************************************************************
#endif // SYSTEM_DEF_H
