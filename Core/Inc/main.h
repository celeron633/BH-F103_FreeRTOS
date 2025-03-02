/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
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

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */
#define HAL_UART_MODULE_ENABLED
/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define KBD_R1_Pin GPIO_PIN_0
#define KBD_R1_GPIO_Port GPIOF
#define KBD_R2_Pin GPIO_PIN_1
#define KBD_R2_GPIO_Port GPIOF
#define KBD_R3_Pin GPIO_PIN_2
#define KBD_R3_GPIO_Port GPIOF
#define KBD_R4_Pin GPIO_PIN_3
#define KBD_R4_GPIO_Port GPIOF
#define LED_G_Pin GPIO_PIN_0
#define LED_G_GPIO_Port GPIOB
#define LED_B_Pin GPIO_PIN_1
#define LED_B_GPIO_Port GPIOB
#define KBD_C1_Pin GPIO_PIN_0
#define KBD_C1_GPIO_Port GPIOD
#define KBD_C2_Pin GPIO_PIN_1
#define KBD_C2_GPIO_Port GPIOD
#define KBD_C3_Pin GPIO_PIN_2
#define KBD_C3_GPIO_Port GPIOD
#define LED_R_Pin GPIO_PIN_5
#define LED_R_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
