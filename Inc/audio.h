/*
 * audio.h
 *
 *  Created on: Apr 23, 2018
 *      Author: benja_000
 */

#ifndef AUDIO_H_
#define AUDIO_H_

#include "definitions.h"
#include "stm32f4xx_hal.h"
#include "math.h"

typedef struct {
  volatile int16_t display[SSD1306_LCDWIDTH];
  volatile int16_t audio[SSD1306_LCDWIDTH];

  volatile float cycle_time;
  volatile float period;

  int16_t num_samples;
  volatile int16_t index;
  int16_t selection;

  uint8_t lfo_x_cal_flag;
  uint8_t lfo_y_cal_flag;
  uint16_t lfo_x_upper_limit;
  uint16_t lfo_x_lower_limit;
  uint16_t lfo_y_upper_limit;
  uint16_t lfo_y_lower_limit;
} WAVEFORM;

#define MAX_FREQUENCY     9000.0
#define MIN_FREQUENCY     5.0

#define LFO_INPUT_UPPER         3070
#define LFO_INPUT_LOWER         1023

#define DISPLAY_TO_DAC          (3103 / SSD1306_LCDHEIGHT)
#define AUDIO_SCALING(X)        (((X) * DISPLAY_TO_DAC) & 0xFFF)

void init_waveform(WAVEFORM *wf, int16_t num_samples);
void reset_waveform(WAVEFORM *wf, int16_t value);
void update_frequency(WAVEFORM *wf, float freq);
void update_sample(WAVEFORM *wf);
void select_waveform(WAVEFORM *wf, int16_t shift);
float_t scale_lfo_x_input(WAVEFORM *wf, uint32_t input);
float_t scale_lfo_y_input(WAVEFORM *wf, uint32_t input);
void lfo_x_cal_sequence(WAVEFORM *wf, uint32_t lfo_input, uint8_t enable);
void lfo_y_cal_sequence(WAVEFORM *wf, uint32_t lfo_input, uint8_t enable);

#endif /* AUDIO_H_ */
