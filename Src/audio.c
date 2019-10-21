/*
 * audio.c
 *
 *  Created on: Apr 23, 2018
 *      Author: benja_000
 */

#include "audio.h"

void init_waveform(WAVEFORM *wf, int16_t num_samples)
{
  wf->cycle_time = 0;
  wf->period = 1.0 / MIN_FREQUENCY;

  wf->num_samples = num_samples;
  wf->index = 0;
  wf->selection = 0;

  wf->lfo_x_upper_limit = LFO_INPUT_UPPER;
  wf->lfo_x_lower_limit = LFO_INPUT_LOWER;
  wf->lfo_y_upper_limit = LFO_INPUT_UPPER;
  wf->lfo_y_lower_limit = LFO_INPUT_LOWER;
  wf->lfo_x_cal_flag = 0;
  wf->lfo_y_cal_flag = 0;
}

void reset_waveform(WAVEFORM *wf, int16_t value)
{
  uint16_t i = 0;
  for(i = 0; i < wf->num_samples; i++) {
    wf->display[i] = value;
    wf->audio[i] = AUDIO_SCALING(value);
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

void select_waveform(WAVEFORM *wf, int16_t shift)
{
  wf->selection += shift;
  wf->selection &= 0x3;
}

float_t scale_lfo_x_input(WAVEFORM *wf, uint32_t input)
{
  return (float_t)(wf->lfo_x_upper_limit - input) / (wf->lfo_x_upper_limit - wf->lfo_x_lower_limit);
}

float_t scale_lfo_y_input(WAVEFORM *wf, uint32_t input)
{
  return (float_t)(wf->lfo_y_upper_limit - input) / (wf->lfo_y_upper_limit - wf->lfo_y_lower_limit);
}

void lfo_x_cal_sequence(WAVEFORM *wf, uint32_t lfo_input, uint8_t enable)
{
  if(enable) {
    if(wf->lfo_x_cal_flag) {
      if(lfo_input > wf->lfo_x_upper_limit) {
        wf->lfo_x_upper_limit = lfo_input;
      }
      else if(lfo_input < wf->lfo_x_lower_limit) {
        wf->lfo_x_lower_limit = lfo_input;
      }
    }
    else {
      wf->lfo_x_upper_limit = lfo_input;
      wf->lfo_x_lower_limit = lfo_input;
      wf->lfo_x_cal_flag = 1;
    }
  }
  else {
    wf->lfo_x_cal_flag = 0;
  }
}

void lfo_y_cal_sequence(WAVEFORM *wf, uint32_t lfo_input, uint8_t enable)
{
  if(enable) {
    if(wf->lfo_y_cal_flag) {
      if(lfo_input > wf->lfo_y_upper_limit) {
        wf->lfo_y_upper_limit = lfo_input;
      }
      else if(lfo_input < wf->lfo_y_lower_limit) {
        wf->lfo_y_lower_limit = lfo_input;
      }
    }
    else {
      wf->lfo_y_upper_limit = lfo_input;
      wf->lfo_y_lower_limit = lfo_input;
      wf->lfo_y_cal_flag = 1;
    }
  }
  else {
    wf->lfo_y_cal_flag = 0;
  }
}
