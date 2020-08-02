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
#include "eeprom.h"
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
LOWPASS fine_filter;
EXT_ADC ext_adc;
/* USER CODE END Variables */
osThreadId defaultTaskHandle;
osThreadId displayTaskHandle;
osThreadId encoderTaskHandle;
osThreadId ledTaskHandle;
osThreadId pitchTaskHandle;
osThreadId buttonTaskHandle;
osThreadId lfoControlTaskHandle;
osSemaphoreId encoderSemaphoreHandle;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */
void draw_sample(void);
void draw_waveform(void);
void save_waveform(uint16_t memory_bank);
void load_waveform(uint16_t memory_bank);
/* USER CODE END FunctionPrototypes */

void DefaultTask(void const * argument);
void DisplayTask(void const * argument);
void EncoderTask(void const * argument);
void LedTask(void const * argument);
void PitchTask(void const * argument);
void ButtonTask(void const * argument);
void LfoControlTask(void const * argument);

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

  /* definition and creation of pitchTask */
  osThreadDef(pitchTask, PitchTask, osPriorityHigh, 0, 128);
  pitchTaskHandle = osThreadCreate(osThread(pitchTask), NULL);

  /* definition and creation of buttonTask */
  osThreadDef(buttonTask, ButtonTask, osPriorityBelowNormal, 0, 128);
  buttonTaskHandle = osThreadCreate(osThread(buttonTask), NULL);

  /* definition and creation of lfoControlTask */
  osThreadDef(lfoControlTask, LfoControlTask, osPriorityBelowNormal, 0, 128);
  lfoControlTaskHandle = osThreadCreate(osThread(lfoControlTask), NULL);

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
    osDelay(10);
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
  init_waveform(&wf, SSD1306_LCDWIDTH);
  load_waveform(wf.selection);

  /* Infinite loop */
  for(;;) {
    // update the display
    display();
    vTaskDelay(pdMS_TO_TICKS(25));
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
  preset_encoder(&encoder_y, (SSD1306_LCDHEIGHT / 2));

  /* Infinite loop */
  for(;;) {
    xSemaphoreTake(encoderSemaphoreHandle, portMAX_DELAY);
    // check both encoders for updated data
    update_encoder(&encoder_x);
    update_encoder(&encoder_y);
    draw_sample();
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

/* USER CODE BEGIN Header_PitchTask */
/**
* @brief Function implementing the pitchTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_PitchTask */
void PitchTask(void const * argument)
{
  /* USER CODE BEGIN PitchTask */
  TickType_t xLastWakeTime;
  float_t pitch_cmd;
//  uint16_t dac_value = 0;
//  uint8_t dac_i2c_data[3] = {DAC_I2C_CONTROL, 0, 0};
  init_lowpass_filter(&note_filter, 20, ADC_SAMPLE_RATE);
  init_lowpass_filter(&fine_filter, 1, ADC_SAMPLE_RATE);
  init_exp_cmd(&freq_cmd, FREQ_CONST_A, FREQ_BASE);
  init_ext_adc(&ext_adc, EXT_ADC_MUX_3, EXT_ADC_MUX_2);
  xLastWakeTime = xTaskGetTickCount();

  /* Infinite loop */
  for(;;)
  {
    vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(20));
    read_ext_adc(&ext_adc);

    lowpass_filter(&fine_filter, g_adc_inputs[ADC_FINE_INDEX]);

    // sum the pitch commands
    pitch_cmd = (CV_SCALAR * (float_t)ext_adc.results[ADC_CV_INDEX]) +
                (COARSE_SCALAR * (float_t)ext_adc.results[ADC_COARSE_INDEX]) +
                (FINE_SCALAR * fine_filter.out);

    lowpass_filter(&note_filter, pitch_cmd);
    exp_cmd(&freq_cmd, note_filter.out);
    update_frequency(&wf, freq_cmd.out);

//    dac_i2c_data[1] = (dac_value >> 8);
//    dac_i2c_data[2] = (dac_value & 0xFF);
//    HAL_I2C_Master_Transmit(&hi2c3, DAC_I2C_WRITE, dac_i2c_data, 3, 1000);
//    HAL_I2C_Master_Transmit_DMA(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint8_t *pData, uint16_t Size);
//    dac_value += 128;
  }
  /* USER CODE END PitchTask */
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
  init_button(&x_button, DEBOUNCE_THRESHOLD, 0, SWITCH_X_GPIO_Port, SWITCH_X_Pin);
  init_button(&y_button, DEBOUNCE_THRESHOLD, 0, SWITCH_Y_GPIO_Port, SWITCH_Y_Pin);
  init_button(&left_button, DEBOUNCE_THRESHOLD, 0, SWITCH_L_GPIO_Port, SWITCH_L_Pin);
  init_button(&right_button, DEBOUNCE_THRESHOLD, 0, SWITCH_R_GPIO_Port, SWITCH_R_Pin);

  init_button_hold(&left_button, HOLD_THRESHOLD);
  init_button_hold(&right_button, HOLD_THRESHOLD);

  /* Infinite loop */
  for(;;)
  {
    vTaskDelay(pdMS_TO_TICKS(10));
    button_debouce(&x_button);
    button_debouce(&y_button);
    button_debouce(&left_button);
    button_debouce(&right_button);

    if(right_button.release) {
      save_waveform(wf.selection);
    }
    else if(left_button.release) {
      reset_waveform(&wf, (SSD1306_LCDHEIGHT / 2));
      draw_waveform();
    }
    else if(x_button.press) {
      select_waveform(&wf, -1);
      load_waveform(wf.selection);
    }
    else if(y_button.press) {
      select_waveform(&wf, 1);
      load_waveform(wf.selection);
    }
  }
  /* USER CODE END ButtonTask */
}

/* USER CODE BEGIN Header_LfoControlTask */
/**
* @brief Function implementing the lfoControlTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_LfoControlTask */
void LfoControlTask(void const * argument)
{
  /* USER CODE BEGIN LfoControlTask */
  vTaskDelay(pdMS_TO_TICKS(100));

  /* Infinite loop */
  for(;;)
  {
    // initiate a new ADC conversion sequence
    HAL_ADC_Start_DMA(&hadc1, g_adc_inputs, ARRAY_SIZE(g_adc_inputs));
    vTaskDelay(pdMS_TO_TICKS(1));
    // at least one of the LFO inputs is connected
    if(g_adc_inputs[ADC_X_INDEX] > DISCONNECT_THRESHOLD ||
       g_adc_inputs[ADC_Y_INDEX] > DISCONNECT_THRESHOLD) {
      // X input connected
      if(g_adc_inputs[ADC_X_INDEX] > DISCONNECT_THRESHOLD) {
        lfo_x_cal_sequence(&wf, g_adc_inputs[ADC_X_INDEX], left_button.hold);
        preset_encoder(&encoder_x, scale_lfo_x_input(&wf, g_adc_inputs[ADC_X_INDEX]) * MAX_WIDTH);
      }
      // Y input connected
      if(g_adc_inputs[ADC_Y_INDEX] > DISCONNECT_THRESHOLD) {
        lfo_y_cal_sequence(&wf, g_adc_inputs[ADC_Y_INDEX], left_button.hold);
        preset_encoder(&encoder_y, scale_lfo_y_input(&wf, g_adc_inputs[ADC_Y_INDEX]) * MAX_HEIGHT);
      }
      draw_sample();
    }
  }
  /* USER CODE END LfoControlTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */
void draw_sample(void)
{
  // update the waveform
  wf.display[encoder_x.position] = encoder_y.position;
  update_audio_sample(&wf, AUDIO_SCALING(encoder_y.position), encoder_x.position);
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
}

void draw_waveform(void)
{
  uint16_t i = 0;
  clear_screen();
  // left screen edge case
  draw_pixel(0, wf.display[0], WHITE);
  // draw remaining lines to generate the waveform
  for(i = 1; i < SSD1306_LCDWIDTH; i++) {
    draw_vline(i, wf.display[i], wf.display[i - 1], WHITE);
  }
}

void save_waveform(uint16_t memory_bank)
{
  uint16_t i = 0, wf_index = 0, start = 0, end = 0;
  memory_bank = (memory_bank < NUM_WAVEFORMS) ? memory_bank : (NUM_WAVEFORMS - 1);
  start = (memory_bank * SSD1306_LCDWIDTH);
  end = start + SSD1306_LCDWIDTH;

  for(i = start; i < end; i++) {
    EE_WriteVariable(i, (uint16_t)wf.display[wf_index]);
    wf_index++;
  }
}

void load_waveform(uint16_t memory_bank)
{
  uint16_t i = 0, wf_index = 0, start = 0, end = 0, fault = 0;
  memory_bank = (memory_bank < NUM_WAVEFORMS) ? memory_bank : (NUM_WAVEFORMS - 1);
  start = (memory_bank * SSD1306_LCDWIDTH);
  end = start + SSD1306_LCDWIDTH;

  for(i = start; i < end; i++) {
    if(fault) {
      for(wf_index = 0; wf_index < SSD1306_LCDWIDTH; wf_index++) {
        wf.display[wf_index] = SSD1306_LCDHEIGHT / 2;
        wf.audio[wf_index] = AUDIO_SCALING(wf.display[wf_index]);
      }
      break;
    }
    else {
      fault = (EE_ReadVariable(i, (uint16_t *)&wf.display[wf_index]) != HAL_OK);
      wf.audio[wf_index] = AUDIO_SCALING(wf.display[wf_index]);
    }
    wf_index++;
  }
  preset_encoder(&encoder_x, 0);
  preset_encoder(&encoder_y, wf.display[0]);
  calculate_deltas(&wf);
  draw_waveform();
}
/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
