/*
 * dsp.h
 *
 *  Created on: Jun 10, 2019
 *      Author: benja_000
 */

#ifndef DSP_H_
#define DSP_H_

#include "stm32f4xx_hal.h"
#include "math.h"

typedef struct {
  // alpha
  float_t alpha;
  // the current scaled filtered output
  volatile float_t previous_out;
  // the current filtered output
  volatile float_t out;
} LOWPASS;

void init_lowpass_filter(LOWPASS *lowpass, float_t cutoff_freq, float_t fs);
void lowpass_filter(LOWPASS *lowpass, float_t in);

#endif /* DSP_H_ */
