/*
 * audio.c
 *
 *  Created on: Apr 23, 2018
 *      Author: benja_000
 */

#include "audio.h"

void init_waveform(WAVEFORM *wf, int16_t max_range, int16_t num_samples, int16_t init)
{
  int16_t i = 0;
  int16_t end = sizeof(wf->display) / sizeof(wf->display[0]);

  wf->cycle_time = 0;
  wf->period = 1.0 / MIN_FREQUENCY;

  wf->num_samples = num_samples;
  wf->index = 0;
  // initialize the waveform
  for(i = 0; i < end; i++) {
    wf->display[i] = init;
    wf->audio[i] = (max_range - init - 1) * max_range;
  }
}

void update_frequency(WAVEFORM *wf, float freq)
{
  freq = LIMIT(freq, MAX_FREQUENCY, MIN_FREQUENCY);
  wf->period = 1.0 / freq;
}

void update_sample(WAVEFORM *wf)
{
  // update cycle time
  wf->cycle_time += DAC_INTERVAL;
  if(wf->cycle_time >= wf->period) {
    wf->cycle_time -= wf->period;
  }
  // update the sample index
  wf->index = (int16_t)((wf->cycle_time / wf->period) * wf->num_samples);
}

