/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
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
#include "stm32f4xx_hal.h"

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
#define SWITCH_X_Pin GPIO_PIN_13
#define SWITCH_X_GPIO_Port GPIOC
#define ENCODER_X1_Pin GPIO_PIN_14
#define ENCODER_X1_GPIO_Port GPIOC
#define ENCODER_X1_EXTI_IRQn EXTI15_10_IRQn
#define ENCODER_X2_Pin GPIO_PIN_15
#define ENCODER_X2_GPIO_Port GPIOC
#define ENCODER_X2_EXTI_IRQn EXTI15_10_IRQn
#define SWITCH_L_Pin GPIO_PIN_6
#define SWITCH_L_GPIO_Port GPIOA
#define SWITCH_R_Pin GPIO_PIN_7
#define SWITCH_R_GPIO_Port GPIOA
#define LED_GREEN_Pin GPIO_PIN_4
#define LED_GREEN_GPIO_Port GPIOC
#define LED_RED_Pin GPIO_PIN_5
#define LED_RED_GPIO_Port GPIOC
#define SWITCH_Y_Pin GPIO_PIN_1
#define SWITCH_Y_GPIO_Port GPIOB
#define ENCODER_Y1_Pin GPIO_PIN_10
#define ENCODER_Y1_GPIO_Port GPIOB
#define ENCODER_Y1_EXTI_IRQn EXTI15_10_IRQn
#define ENCODER_Y2_Pin GPIO_PIN_11
#define ENCODER_Y2_GPIO_Port GPIOB
#define ENCODER_Y2_EXTI_IRQn EXTI15_10_IRQn
#define ADC_SCK_Pin GPIO_PIN_13
#define ADC_SCK_GPIO_Port GPIOB
#define ADC_MISO_Pin GPIO_PIN_14
#define ADC_MISO_GPIO_Port GPIOB
#define ADC_MOSI_Pin GPIO_PIN_15
#define ADC_MOSI_GPIO_Port GPIOB
#define ADC_CS_Pin GPIO_PIN_6
#define ADC_CS_GPIO_Port GPIOC
#define DISPLAY_CS_Pin GPIO_PIN_15
#define DISPLAY_CS_GPIO_Port GPIOA
#define DISPLAY_SCK_Pin GPIO_PIN_10
#define DISPLAY_SCK_GPIO_Port GPIOC
#define DISPLAY_MOSI_Pin GPIO_PIN_12
#define DISPLAY_MOSI_GPIO_Port GPIOC
#define DISPLAY_RST_Pin GPIO_PIN_2
#define DISPLAY_RST_GPIO_Port GPIOD
#define DISPLAY_DC_Pin GPIO_PIN_4
#define DISPLAY_DC_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
