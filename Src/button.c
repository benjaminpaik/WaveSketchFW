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
  button->polarity = polarity;
  button->port = port;
  button->pin = pin;

  button->out = HAL_GPIO_ReadPin(port, pin);
  button->current = button->out;
  button->previous = button->out;
  button->count = 0;
  button->presses = 0;
}

void button_debouce(BUTTON *button)
{
  button->current = HAL_GPIO_ReadPin(button->port, button->pin);

  if(button->current != button->previous) {
    button->count = 0;
  }
  else {
    if(button->count < button->threshold) {
      button->count++;
    }
    else {
      button->out = (button->polarity ? button->current : !button->current);
    }
  }
  button->previous = button->current;
}
