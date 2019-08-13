/*
 * control.h
 *
 *  Created on: Sep 13, 2018
 *      Author: benja_000
 */

#ifndef CONTROL_H_
#define CONTROL_H_

#include "stm32f4xx_hal.h"
#include "math.h"

typedef struct {
    float_t f0;
    float_t a;
    float_t out;
} EXP_CMD;

#define INPUT_LIMIT     125

void init_exp_cmd(EXP_CMD *exp_cmd, float_t a, float_t f0);
void exp_cmd(EXP_CMD *exp_cmd, float_t input);

#endif /* CONTROL_H_ */
