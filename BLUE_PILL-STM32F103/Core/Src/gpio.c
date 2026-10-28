/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    gpio.c
  * @brief   This file provides code for the configuration
  *          of all used GPIO pins.
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
#include "gpio.h"

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/*----------------------------------------------------------------------------*/
/* Configure GPIO                                                             */
/*----------------------------------------------------------------------------*/
/* USER CODE BEGIN 1 */

/* USER CODE END 1 */

/** Configure pins as
        * Analog
        * Input
        * Output
        * EVENT_OUT
        * EXTI
*/
void MX_GPIO_Init(void)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(PC13_OLED_MKP2_GPIO_Port, PC13_OLED_MKP2_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, PB12_TEST0_MKP25_Pin|PB13_TEST1_MKP26_Pin|PB14_TEST2_MKP27_Pin|PB15_TEST3_MKP28_Pin
                          |PB4_KEY1R_MKP40_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, PB7_RLED_MKP43_Pin|PB8_GLED_MKP45_Pin|PB9_BLED_MKP46_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin : PC13_OLED_MKP2_Pin */
  GPIO_InitStruct.Pin = PC13_OLED_MKP2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(PC13_OLED_MKP2_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : PB12_TEST0_MKP25_Pin PB13_TEST1_MKP26_Pin PB14_TEST2_MKP27_Pin PB15_TEST3_MKP28_Pin */
  GPIO_InitStruct.Pin = PB12_TEST0_MKP25_Pin|PB13_TEST1_MKP26_Pin|PB14_TEST2_MKP27_Pin|PB15_TEST3_MKP28_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : PB4_KEY1R_MKP40_Pin PB7_RLED_MKP43_Pin PB8_GLED_MKP45_Pin PB9_BLED_MKP46_Pin */
  GPIO_InitStruct.Pin = PB4_KEY1R_MKP40_Pin|PB7_RLED_MKP43_Pin|PB8_GLED_MKP45_Pin|PB9_BLED_MKP46_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : PB5_KEY1_MKP41_Pin */
  GPIO_InitStruct.Pin = PB5_KEY1_MKP41_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(PB5_KEY1_MKP41_GPIO_Port, &GPIO_InitStruct);

}

/* USER CODE BEGIN 2 */

/* USER CODE END 2 */
