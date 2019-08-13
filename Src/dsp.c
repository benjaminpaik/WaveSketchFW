/*
 * dsp.c
 *
 *  Created on: Jun 10, 2019
 *      Author: benja_000
 */

#include "dsp.h"

void init_lowpass_filter(LOWPASS *lowpass, float_t cutoff_freq, float_t fs)
{
  // declare variables
  float_t dt = 0;
  float_t wc = 0;
  float_t alpha = 0;
  // check nyquist bounds
  if(((2 * cutoff_freq) > fs)) {
    cutoff_freq = fs / 2;
  }
  // enable filter if cutoff frequency is greater than zero
  if(cutoff_freq > 0) {
    // calculate intermediate variables
    dt = 1.0 / fs;
    wc = 2.0 * M_PI * cutoff_freq;
    alpha = dt / ((1.0 / wc) + dt);
  }
  // initialize structure variables
  lowpass->alpha = alpha;
  lowpass->out = 0;
  lowpass->previous_out = 0;
}

void lowpass_filter(LOWPASS *lowpass, float_t in)
{
  // check if the filter is enabled
  if(lowpass->alpha) {
    // filtering algorithm
    lowpass->out = lowpass->previous_out + (lowpass->alpha * (in - lowpass->previous_out));
    // store the previous output
    lowpass->previous_out = lowpass->out;
  }
  else {
    // set the lowpass filter input
    lowpass->out = in;
  }
}
