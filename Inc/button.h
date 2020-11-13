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
    uint16_t out_threshold;
    uint16_t count;

    uint8_t current;
    uint8_t previous;
    uint8_t out;
    uint8_t out_previous;

    uint8_t out_rising;
    uint8_t out_falling;

    uint16_t hold_threshold;
    uint8_t hold;
    uint8_t hold_previous;

    uint8_t hold_rising;
    uint8_t hold_falling;
} BUTTON;

#define HOLD_DISABLED     3

void init_button(BUTTON *button, uint16_t threshold, uint8_t polarity, GPIO_TypeDef *port, uint16_t pin);
void init_button_hold(BUTTON *button, uint16_t hold_threshold);
void button_debouce(BUTTON *button);
void button_hold(BUTTON *button);

#endif /* BUTTON_H_ */
