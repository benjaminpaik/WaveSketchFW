/*
 * encoder.c
 *
 *  Created on: Oct 7, 2017
 *      Author: benja_000
 */

#include "encoder.h"
#include "stm32f4xx_hal_gpio.h"
#include "definitions.h"

void init_encoder(ENCODER *encoder, GPIO_TypeDef *port_a, uint16_t pin_a, GPIO_TypeDef *port_b, uint16_t pin_b, int16_t upper_limit, int16_t lower_limit)
{
  encoder->state = 0;
  encoder->port_a = port_a;
  encoder->pin_a = pin_a;
  encoder->port_b = port_b;
  encoder->pin_b = pin_b;

  encoder->upper_limit = upper_limit;
  encoder->lower_limit = lower_limit;
  encoder->integrator_upper_limit = upper_limit << ENCODER_SENSITIVITY;
  encoder->integrator_lower_limit = lower_limit << ENCODER_SENSITIVITY;
  encoder->sensitivity = ENCODER_SENSITIVITY;
}

void preset_encoder(ENCODER *encoder, int16_t position)
{
  encoder->position = LIMIT(position, encoder->upper_limit, encoder->lower_limit);
  encoder->integrator = encoder->position << encoder->sensitivity;
}

void update_encoder(ENCODER *encoder)
{
  // find encoder state and perform grey code conversion
  if(HAL_GPIO_ReadPin(encoder->port_b, encoder->pin_b)) {
    if(HAL_GPIO_ReadPin(encoder->port_a, encoder->pin_a)) encoder->state = 2;
    else encoder->state = 3;
  }
  else {
    if(HAL_GPIO_ReadPin(encoder->port_a, encoder->pin_a)) encoder->state = 1;
    else encoder->state = 0;
  }

  // calculate delta and handle roll-over
  encoder->delta = encoder->state - encoder->previous_state;
  if(encoder->delta > 2) encoder->delta -= 4;
  else if(encoder->delta < -2) encoder->delta += 4;

  // integrate position change
  if(encoder->delta > 0) encoder->integrator++;
  else if(encoder->delta < 0) encoder->integrator--;
  // set upper and lower integrator limits
  encoder->integrator = LIMIT(encoder->integrator, encoder->integrator_upper_limit, encoder->integrator_lower_limit);
  // scale down the encoder position
  encoder->position = encoder->integrator >> encoder->sensitivity;
  // store the previous state
  encoder->previous_state = encoder->state;
}
