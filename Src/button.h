/*
 * button.h
 *
 *  Created on: May 30, 2019
 *      Author: benja_000
 */

#ifndef BUTTON_H_
#define BUTTON_H_

#include "definitions.h"
#include "stm32f4xx_hal.h"

typedef struct {
    GPIO_TypeDef *port;
    uint16_t pin;

    uint8_t polarity;
    uint16_t threshold;
    uint16_t count;
    uint16_t presses;

    uint8_t current;
    uint8_t previous;
    uint8_t out;
} BUTTON;

void init_button(BUTTON *button, uint16_t threshold, uint8_t polarity, GPIO_TypeDef *port, uint16_t pin);
void button_debouce(BUTTON *button);

#endif /* BUTTON_H_ */
