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
#include "stm32l4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

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
#define B1_Pin GPIO_PIN_13
#define B1_GPIO_Port GPIOC
#define USART_TX_Pin GPIO_PIN_2
#define USART_TX_GPIO_Port GPIOA
#define USART_RX_Pin GPIO_PIN_3
#define USART_RX_GPIO_Port GPIOA
#define LD2_Pin GPIO_PIN_5
#define LD2_GPIO_Port GPIOA
#define TMS_Pin GPIO_PIN_13
#define TMS_GPIO_Port GPIOA
#define TCK_Pin GPIO_PIN_14
#define TCK_GPIO_Port GPIOA
#define SWO_Pin GPIO_PIN_3
#define SWO_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */
/* RC受信入力 (TIM3 入力キャプチャ, Futaba R3206SBM想定) */
#define RC_AIL_Pin GPIO_PIN_6
#define RC_AIL_GPIO_Port GPIOA
#define RC_ELE_Pin GPIO_PIN_7
#define RC_ELE_GPIO_Port GPIOA
#define RC_RUD_Pin GPIO_PIN_0
#define RC_RUD_GPIO_Port GPIOB
#define RC_THR_Pin GPIO_PIN_1
#define RC_THR_GPIO_Port GPIOB

/* PWM出力 (TIM1, サーボ/ESCへのパススルー出力) */
#define PWM_AIL_Pin GPIO_PIN_8
#define PWM_AIL_GPIO_Port GPIOA
#define PWM_ELE_Pin GPIO_PIN_9
#define PWM_ELE_GPIO_Port GPIOA
#define PWM_RUD_Pin GPIO_PIN_10
#define PWM_RUD_GPIO_Port GPIOA
#define PWM_THR_Pin GPIO_PIN_11
#define PWM_THR_GPIO_Port GPIOA
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
