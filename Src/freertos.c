/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */     
#include "main.h"
#include "adc.h"
#include "i2c.h"
#include "display.h"
#include "audio.h"
#include "encoder.h"
#include "control.h"
#include "button.h"
#include "semphr.h"
#include "dsp.h"
#include "ext_adc.h"
#include "definitions.h"
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
/* USER CODE BEGIN Variables */
WAVEFORM wf;
ENCODER encoder_x;
ENCODER encoder_y;
BUTTON x_button;
BUTTON y_button;
BUTTON left_button;
BUTTON right_button;
EXP_CMD freq_cmd;
uint32_t g_adc_inputs[5];
LOWPASS note_filter;
EXT_ADC ext_adc;
/* USER CODE END Variables */
osThreadId defaultTaskHandle;
osThreadId displayTaskHandle;
osThreadId encoderTaskHandle;
osThreadId ledTaskHandle;
osThreadId adcTaskHandle;
osThreadId buttonTaskHandle;
osSemaphoreId displaySemaphoreHandle;
osSemaphoreId encoderSemaphoreHandle;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */
   
/* USER CODE END FunctionPrototypes */

void DefaultTask(void const * argument);
void DisplayTask(void const * argument);
void EncoderTask(void const * argument);
void LedTask(void const * argument);
void AdcTask(void const * argument);
void ButtonTask(void const * argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */
       
  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* Create the semaphores(s) */
  /* definition and creation of displaySemaphore */
  osSemaphoreDef(displaySemaphore);
  displaySemaphoreHandle = osSemaphoreCreate(osSemaphore(displaySemaphore), 1);

  /* definition and creation of encoderSemaphore */
  osSemaphoreDef(encoderSemaphore);
  encoderSemaphoreHandle = osSemaphoreCreate(osSemaphore(encoderSemaphore), 1);

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* definition and creation of defaultTask */
  osThreadDef(defaultTask, DefaultTask, osPriorityNormal, 0, 128);
  defaultTaskHandle = osThreadCreate(osThread(defaultTask), NULL);

  /* definition and creation of displayTask */
  osThreadDef(displayTask, DisplayTask, osPriorityLow, 0, 128);
  displayTaskHandle = osThreadCreate(osThread(displayTask), NULL);

  /* definition and creation of encoderTask */
  osThreadDef(encoderTask, EncoderTask, osPriorityNormal, 0, 128);
  encoderTaskHandle = osThreadCreate(osThread(encoderTask), NULL);

  /* definition and creation of ledTask */
  osThreadDef(ledTask, LedTask, osPriorityLow, 0, 128);
  ledTaskHandle = osThreadCreate(osThread(ledTask), NULL);

  /* definition and creation of adcTask */
  osThreadDef(adcTask, AdcTask, osPriorityHigh, 0, 128);
  adcTaskHandle = osThreadCreate(osThread(adcTask), NULL);

  /* definition and creation of buttonTask */
  osThreadDef(buttonTask, ButtonTask, osPriorityBelowNormal, 0, 128);
  buttonTaskHandle = osThreadCreate(osThread(buttonTask), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

}

/* USER CODE BEGIN Header_DefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used 
  * @retval None
  */
/* USER CODE END Header_DefaultTask */
void DefaultTask(void const * argument)
{
    
    
    
    
    
    
    

  /* USER CODE BEGIN DefaultTask */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END DefaultTask */
}

/* USER CODE BEGIN Header_DisplayTask */
/**
* @brief Function implementing the displayTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_DisplayTask */
void DisplayTask(void const * argument)
{
  /* USER CODE BEGIN DisplayTask */
  // initialize the OLED display
  init_display(0, 1);
  clear_screen();
  draw_hline((SSD1306_LCDHEIGHT >> 1), MAX_WIDTH, 0, WHITE);
  init_waveform(&wf, SSD1306_LCDHEIGHT, SSD1306_LCDWIDTH, (SSD1306_LCDHEIGHT / 2));

  /* Infinite loop */
  for(;;) {
    if(displaySemaphoreHandle != NULL) {
      // block until the semaphore is available
      if(xSemaphoreTake(displaySemaphoreHandle, portMAX_DELAY) == pdTRUE) {
        // update the display
        display();
      }
    }
  }
  /* USER CODE END DisplayTask */
}

/* USER CODE BEGIN Header_EncoderTask */
/**
* @brief Function implementing the encoderTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_EncoderTask */
void EncoderTask(void const * argument)
{
  /* USER CODE BEGIN EncoderTask */
  // initialize the rotary encoders
  init_encoder(&encoder_x, ENCODER_X1_GPIO_Port, ENCODER_X1_Pin, ENCODER_X2_GPIO_Port, ENCODER_X2_Pin, MAX_WIDTH, 0);
  init_encoder(&encoder_y, ENCODER_Y2_GPIO_Port, ENCODER_Y2_Pin, ENCODER_Y1_GPIO_Port, ENCODER_Y1_Pin, MAX_HEIGHT, 0);
  // initialize encoder positions
  preset_encoder(&encoder_x, 0);
  preset_encoder(&encoder_y, (SSD1306_LCDHEIGHT >> 1));

  /* Infinite loop */
  for(;;) {
    xSemaphoreTake(encoderSemaphoreHandle, portMAX_DELAY);
    // check both encoders for updated data
    update_encoder(&encoder_x);
    update_encoder(&encoder_y);
    // update the waveform
    wf.display[encoder_x.position] = encoder_y.position;
    wf.audio[encoder_x.position] = AUDIO_SCALING(encoder_y.position);
    // clear the current and adjacent columns
    draw_vline(encoder_x.position, MAX_HEIGHT, 0, BLACK);
    draw_vline(encoder_x.position + 1, MAX_HEIGHT, 0, BLACK);

    // draw current vertical line
    if(encoder_x.position > 0) {
      // standard case
      draw_vline(encoder_x.position, encoder_y.position, wf.display[encoder_x.position - 1], WHITE);
    }
    else {
      // left screen edge case
      draw_pixel(encoder_x.position, encoder_y.position, WHITE);
    }

    // draw adjacent vertical line
    if(encoder_x.position < MAX_WIDTH) {
      draw_vline(encoder_x.position + 1, encoder_y.position, wf.display[encoder_x.position + 1], WHITE);
    }
    // give the semaphore to the display task
    xSemaphoreGive(displaySemaphoreHandle);
  }
  /* USER CODE END EncoderTask */
}

/* USER CODE BEGIN Header_LedTask */
/**
* @brief Function implementing the ledTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_LedTask */
void LedTask(void const * argument)
{
  /* USER CODE BEGIN LedTask */
  TickType_t xLastWakeTime;
  xLastWakeTime = xTaskGetTickCount();

  /* Infinite loop */
  for(;;) {
    HAL_GPIO_WritePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(LED_RED_GPIO_Port, LED_RED_Pin, GPIO_PIN_RESET);
    vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(250));

    HAL_GPIO_WritePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(LED_RED_GPIO_Port, LED_RED_Pin, GPIO_PIN_SET);
    vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(250));
  }
  /* USER CODE END LedTask */
}

/* USER CODE BEGIN Header_AdcTask */
/**
* @brief Function implementing the adcTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_AdcTask */
void AdcTask(void const * argument)
{
  /* USER CODE BEGIN AdcTask */
  TickType_t xLastWakeTime;
  float_t pitch_cmd;
//  uint16_t dac_value = 0;
//  uint8_t dac_i2c_data[3] = {DAC_I2C_CONTROL, 0, 0};
  init_lowpass_filter(&note_filter, 20, ADC_SAMPLE_RATE);
  init_exp_cmd(&freq_cmd, FREQ_CONST_A, FREQ_BASE);
  init_ext_adc(&ext_adc, EXT_ADC_MUX_3, EXT_ADC_MUX_2);
  xLastWakeTime = xTaskGetTickCount();

  /* Infinite loop */
  for(;;)
  {
    vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(20));
    read_ext_adc(&ext_adc);

    // sum the pitch commands
    pitch_cmd = (CV_SCALAR * (float_t)ext_adc.results[ADC_CV_INDEX]) +
                (COARSE_SCALAR * (float_t)ext_adc.results[ADC_COARSE_INDEX]) +
                (FINE_SCALAR * (float_t)g_adc_inputs[ADC_FINE_INDEX]);
    lowpass_filter(&note_filter, pitch_cmd);
    exp_cmd(&freq_cmd, note_filter.out);
    update_frequency(&wf, freq_cmd.out);
    // initiate a new ADC conversion sequence
    HAL_ADC_Start_DMA(&hadc1, g_adc_inputs, ARRAY_SIZE(g_adc_inputs));

//    dac_i2c_data[1] = (dac_value >> 8);
//    dac_i2c_data[2] = (dac_value & 0xFF);
//    HAL_I2C_Master_Transmit(&hi2c1, DAC_I2C_WRITE, dac_i2c_data, 3, 1000);
//    HAL_I2C_Master_Transmit_DMA(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint8_t *pData, uint16_t Size);

//    dac_value += 128;
  }
  /* USER CODE END AdcTask */
}

/* USER CODE BEGIN Header_ButtonTask */
/**
* @brief Function implementing the buttonTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_ButtonTask */
void ButtonTask(void const * argument)
{
  /* USER CODE BEGIN ButtonTask */
  TickType_t xLastWakeTime;
  init_button(&x_button, DEBOUNCE_THRESHOLD, 0, SWITCH_X_GPIO_Port, SWITCH_X_Pin);
  init_button(&y_button, DEBOUNCE_THRESHOLD, 0, SWITCH_Y_GPIO_Port, SWITCH_Y_Pin);
  init_button(&left_button, DEBOUNCE_THRESHOLD, 1, SWITCH_L_GPIO_Port, SWITCH_L_Pin);
  init_button(&right_button, DEBOUNCE_THRESHOLD, 1, SWITCH_R_GPIO_Port, SWITCH_R_Pin);
  xLastWakeTime = xTaskGetTickCount();

  /* Infinite loop */
  for(;;)
  {
    vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(10));
    button_debouce(&x_button);
    button_debouce(&y_button);
    button_debouce(&left_button);
    button_debouce(&right_button);
  }
  /* USER CODE END ButtonTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */
     
/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
