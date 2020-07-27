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
  wf->sample = 0;
  wf->slope = 0;
  wf->sample_time = 1.0;

  wf->period_buffer = 1.0;
  wf->sample_time_buffer = 1.0;

  wf->lfo_x_upper_limit = LFO_INPUT_UPPER;
  wf->lfo_x_lower_limit = LFO_INPUT_LOWER;
  wf->lfo_y_upper_limit = LFO_INPUT_UPPER;
  wf->lfo_y_lower_limit = LFO_INPUT_LOWER;
  wf->lfo_x_cal_flag = 0;
  wf->lfo_y_cal_flag = 0;
}

void reset_waveform(WAVEFORM *wf, int16_t value)
{
  for(int16_t i = 0; i < wf->num_samples; i++) {
    wf->display[i] = value;
    wf->audio[i] = AUDIO_SCALING(value);
    wf->delta[i] = 0;
  }
}

void update_audio_sample(WAVEFORM *wf, int16_t sample, int16_t index)
{
  int16_t next_index, previous_index;

  next_index = (index == (wf->num_samples - 1)) ? 0 : (index + 1);
  previous_index = (index == 0) ? (wf->num_samples - 1) : (index - 1);

  wf->audio[index] = sample;
  wf->delta[index] = wf->audio[next_index] - sample;
  wf->delta[previous_index] = sample - wf->audio[previous_index];
}

void calculate_deltas(WAVEFORM *wf)
{
  int16_t last_index = wf->num_samples - 1;
  for(int16_t i = 0; i < last_index; i++) {
    wf->delta[i] = wf->audio[i + 1] - wf->audio[i];
  }
  wf->delta[last_index] = wf->audio[0] - wf->audio[last_index];
}

void update_frequency(WAVEFORM *wf, float freq)
{
  freq = LIMIT(freq, MAX_FREQUENCY, MIN_FREQUENCY);
  wf->period = 1.0 / freq;
  wf->sample_time = wf->period / wf->num_samples;
}

void update_sample(WAVEFORM *wf)
{
  // update cycle time
  wf->cycle_time += DAC_INTERVAL;
  if(wf->cycle_time >= wf->period_buffer) {
    wf->cycle_time -= wf->period_buffer;
//    wf->cycle_time = 0;
    wf->period_buffer = wf->period;
    wf->sample_time_buffer = wf->sample_time;
  }
  // update the sample index
  wf->cycle_percent = wf->cycle_time / wf->period_buffer;
  wf->index = (int16_t)(wf->cycle_percent * wf->num_samples);
  wf->slope = wf->delta[wf->index] / wf->sample_time_buffer;
  wf->sample = wf->audio[wf->index] + (wf->slope * (wf->cycle_time - (wf->sample_time_buffer * wf->index)));
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
      wf->lfo_x_upper_limit = lfo_input + 1;
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
      wf->lfo_y_upper_limit = lfo_input + 1;
      wf->lfo_y_lower_limit = lfo_input;
      wf->lfo_y_cal_flag = 1;
    }
  }
  else {
    wf->lfo_y_cal_flag = 0;
  }
}
