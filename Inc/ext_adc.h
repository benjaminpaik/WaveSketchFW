/*
 * ext_adc.h
 *
 *  Created on: Aug 12, 2019
 *      Author: benja_000
 */

#ifndef EXT_ADC_H_
#define EXT_ADC_H_

#include "stm32f405xx.h"

typedef union {
  struct {
    uint16_t reserved:1;
    uint16_t nop:2;
    uint16_t pullup_enable:1;
    uint16_t ts_mode:1;
    uint16_t data_rate:3;
    uint16_t mode:1;
    uint16_t pga:3;
    uint16_t mux:3;
    uint16_t ss:1;
  } bit;
  uint16_t all;
} EXT_ADC_CONFIG;

typedef struct {
  EXT_ADC_CONFIG config;

  uint16_t upper_mux;
  uint16_t lower_mux;
  uint16_t active_mux;

  uint16_t result_index;
  int16_t results[4];
  uint16_t tx_buffer[2];
  int16_t rx_buffer[2];
} EXT_ADC;

#define EXT_ADC_MUX_0   0x4
#define EXT_ADC_MUX_1   0x5
#define EXT_ADC_MUX_2   0x6
#define EXT_ADC_MUX_3   0x7

void init_ext_adc(EXT_ADC *ext_adc, uint16_t upper_mux, uint16_t lower_mux);
void read_ext_adc(EXT_ADC *ext_adc);

#endif /* EXT_ADC_H_ */
