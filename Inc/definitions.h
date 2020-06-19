/*
 * definitions.h
 *
 *  Created on: Oct 7, 2017
 *      Author: benja_000
 */

#ifndef DEFINITIONS_H_
#define DEFINITIONS_H_

#include "stm32f4xx_hal.h"

#define EEPROM_SIZE             (128 * 4)
#define NUM_WAVEFORMS           4

#define FREQ_BASE               2.0F
#define FREQ_CONST_A            1.0594631F

#define CV_SCALAR               0.003F
#define COARSE_SCALAR           0.003F
#define FINE_SCALAR             0.002F

#define HOLD_THRESHOLD          100
#define DEBOUNCE_THRESHOLD      4
#define DISCONNECT_THRESHOLD    300

#define ADC_SAMPLE_RATE         50
#define COARSE_CUTOFF           10
#define FINE_CUTOFF             10
#define CV_CUTOFF               10

#define ADC_X_INDEX             0
#define ADC_Y_INDEX             1
#define ADC_CV_INDEX            2
#define ADC_COARSE_INDEX        3
#define ADC_FINE_INDEX          4

#define DAC_RATE                96000.0
#define DAC_INTERVAL            (1.0 / DAC_RATE)
#define DAC_I2C_WRITE           (0x98)
#define DAC_I2C_CONTROL         (0x10)

#define ARRAY_SIZE(X)           (sizeof(X) / sizeof(X[0]))

// select display resolution
#define SSD1306_128_64

#if defined SSD1306_128_64
  #define SSD1306_LCDWIDTH                  128
  #define SSD1306_LCDHEIGHT                 64
#endif
#if defined SSD1306_128_32
  #define SSD1306_LCDWIDTH                  128
  #define SSD1306_LCDHEIGHT                 32
#endif
#if defined SSD1306_96_16
  #define SSD1306_LCDWIDTH                  96
  #define SSD1306_LCDHEIGHT                 16
#endif

// limit macro
#define LIMIT(xx, ulim, llim) (((xx)>(ulim))?(ulim):(((xx)<(llim))?(llim):(xx)))

#endif /* DEFINITIONS_H_ */
