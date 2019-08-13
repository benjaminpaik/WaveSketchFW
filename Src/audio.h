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

typedef struct {
  volatile int16_t display[SSD1306_LCDWIDTH];
  volatile int16_t audio[SSD1306_LCDWIDTH];

  volatile float cycle_time;
  volatile float period;

  int16_t num_samples;
  volatile int16_t index;
} WAVEFORM;

#define MAX_FREQUENCY     9000.0
#define MIN_FREQUENCY     5.0

#define DISPLAY_TO_DAC          (3103 / SSD1306_LCDHEIGHT)
#define AUDIO_SCALING(X)        (((X) * DISPLAY_TO_DAC) & 0xFFF)

void init_waveform(WAVEFORM *wf, int16_t max_range, int16_t num_samples, int16_t init);
void update_frequency(WAVEFORM *wf, float freq);
void update_sample(WAVEFORM *wf);

#endif /* AUDIO_H_ */
