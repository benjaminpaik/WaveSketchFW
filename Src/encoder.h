/*
 * encoder.h
 *
 *  Created on: Oct 7, 2017
 *      Author: benja_000
 */

#ifndef ENCODER_H_
#define ENCODER_H_

#include "stm32f4xx_hal.h"
#include "stm32f405xx.h"

typedef struct {
  volatile int8_t delta;
  volatile int16_t integrator;
  volatile int16_t position;
  volatile int8_t previous_state;
  volatile int8_t state;

  GPIO_TypeDef *port_a;
  GPIO_TypeDef *port_b;
  uint8_t sensitivity;
  int16_t upper_limit;
  int16_t lower_limit;
  uint16_t pin_a;
  uint16_t pin_b;
} ENCODER;

// encoder sensitivity
#define ENCODER_SENSITIVITY     2

void init_encoder(ENCODER *encoder, GPIO_TypeDef *port_a, uint16_t pin_a, GPIO_TypeDef *port_b, uint16_t pin_b, int16_t upper_limit, int16_t lower_limit);
void preset_encoder(ENCODER *encoder, int16_t position);
void update_encoder(ENCODER *encoder);

#endif /* ENCODER_H_ */
