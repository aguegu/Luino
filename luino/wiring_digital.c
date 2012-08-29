/*
 * wiring_digital.c
 *
 *  Created on: 2012-8-29
 *      Author: Agu
 */

#define LUINO_MAIN

#include "wiring_private.h"
#include "pins_luino.h"

void pinMode(uint8_t pin, uint8_t mode)
{
	uint8_t bit = digitalPinToBitMask(pin);
	uint8_t port = digitalPinToPort(pin);
	volatile uint8_t *reg, *out;

	if (port == NOT_A_PIN) return;

	// JWS: can I let the optimizer do this?
	reg = portModeRegister(port);
	out = portOutputRegister(port);

	uint8_t oldSREG = SREG;
            cli();

	if (mode == INPUT) {
		*reg &= ~bit;
		*out &= ~bit;
	} else if (mode == INPUT_PULLUP) {
		*reg &= ~bit;
		*out |= bit;
	} else {
		*reg |= bit;
	}

	SREG = oldSREG;
}

