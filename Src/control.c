/*
 * control.c
 *
 *  Created on: Sep 13, 2018
 *      Author: benja_000
 */
#include "control.h"
#include "math.h"

void init_exp_cmd(EXP_CMD *exp_cmd, float_t a, float_t f0)
{
  exp_cmd->a = a;
  exp_cmd->f0 = f0;
  exp_cmd->out = f0;
}

void exp_cmd(EXP_CMD *exp_cmd, float_t input)
{
  if(input > INPUT_LIMIT) {
    input = INPUT_LIMIT;
  }
  exp_cmd->out = exp_cmd->f0 * pow(exp_cmd->a, input);
}
