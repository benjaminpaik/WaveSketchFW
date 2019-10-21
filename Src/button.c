/*
 * button.c
 *
 *  Created on: May 30, 2019
 *      Author: benja_000
 */

#include "button.h"

void init_button(BUTTON *button, uint16_t threshold, uint8_t polarity, GPIO_TypeDef *port, uint16_t pin)
{
  button->threshold = threshold;
  button->hold_threshold = 0;
  button->polarity = polarity;
  button->port = port;
  button->pin = pin;

  button->out = 0;
  button->current = 0;
  button->previous = 0;
  button->count = 0;
  button->hold_count = 0;
  button->hold_threshold = 0;
  button->hold = 0;
  button->hold_previous = 0;
}

void init_button_hold(BUTTON *button, uint16_t hold_threshold)
{
  button->hold_threshold = hold_threshold;
}

void button_debouce(BUTTON *button)
{
  button->current = HAL_GPIO_ReadPin(button->port, button->pin);

  if(button->current != button->previous) {
    button->count = 0;
    button->hold_count = 0;
  }
  else if(button->current == button->polarity) {
    if(button->count < button->threshold) {
      button->count++;
    }
    if(button->hold_count < button->hold_threshold) {
      button->hold_count++;
    }
  }

  button->out = (button->count == button->threshold);
  button->hold = (button->hold_count == button->hold_threshold);

  if(button->hold == button->hold_previous) {
    button->press = (button->out && !button->out_previous);
    button->release = (!button->out && button->out_previous);
  }
  button->out_previous = button->out;
  button->hold_previous = button->hold;
  button->previous = button->current;
}
