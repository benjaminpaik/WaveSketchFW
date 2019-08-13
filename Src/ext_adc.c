/*
 * ext_adc.c
 *
 *  Created on: Aug 12, 2019
 *      Author: benja_000
 */

#include "ext_adc.h"
#include "spi.h"
#include "main.h"

void init_ext_adc(EXT_ADC *ext_adc, uint16_t upper_mux, uint16_t lower_mux)
{
  // clear all bits
  ext_adc->config.all = 0;
  // write all bits
  ext_adc->config.bit.ss = 1;
  ext_adc->config.bit.mux = 0;
  ext_adc->config.bit.pga = 0x1;
  ext_adc->config.bit.mode = 1;
  ext_adc->config.bit.data_rate = 0x4;
  ext_adc->config.bit.ts_mode = 0;
  ext_adc->config.bit.pullup_enable = 1;
  ext_adc->config.bit.nop = 0x1;
  ext_adc->config.bit.reserved = 0x1;
  HAL_GPIO_WritePin(ADC_CS_GPIO_Port, ADC_CS_Pin, GPIO_PIN_RESET);


  ext_adc->upper_mux = (upper_mux > EXT_ADC_MUX_3) ? EXT_ADC_MUX_3 : upper_mux;
  ext_adc->lower_mux = (lower_mux > upper_mux) ? upper_mux : lower_mux;
  ext_adc->active_mux = ext_adc->lower_mux;

  ext_adc->result_index = 0;
}

void read_ext_adc(EXT_ADC *ext_adc)
{
  // set CS LOW
  HAL_GPIO_WritePin(ADC_CS_GPIO_Port, ADC_CS_Pin, GPIO_PIN_RESET);
  // block until MISO is pulled LOW
//  while(HAL_GPIO_ReadPin(EXT_ADC_MISO_GPIO_Port, EXT_ADC_MOSI_Pin) != GPIO_PIN_RESET);

  // update the ADC MUX
  if(++ext_adc->active_mux > ext_adc->upper_mux) {
    ext_adc->active_mux = ext_adc->lower_mux;
  }

  // load the configuration
  ext_adc->config.bit.mux = ext_adc->active_mux;
  ext_adc->tx_buffer[0] = ext_adc->config.all;
  ext_adc->tx_buffer[1] = ext_adc->config.all;

  HAL_SPI_TransmitReceive(&hspi3, (uint8_t*)ext_adc->tx_buffer, (uint8_t*)ext_adc->rx_buffer, 2, 0xFFFFFF);

  // store the latest ADC result
  ext_adc->results[ext_adc->result_index] = ext_adc->rx_buffer[0];
  // store the result index
  ext_adc->result_index = ext_adc->config.bit.mux - EXT_ADC_MUX_0;
  // set CS HIGH
  HAL_GPIO_WritePin(ADC_CS_GPIO_Port, ADC_CS_Pin, GPIO_PIN_SET);
}

