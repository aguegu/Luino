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

	if (port == NOT_A_PIN)
		return;

	// JWS: can I let the optimizer do this?
	reg = portModeRegister(port);
	out = portOutputRegister(port);

	uint8_t oldSREG = SREG;
	cli();

	if (mode == INPUT)
	{
		*reg &= ~bit;
		*out &= ~bit;
	}
	else if (mode == INPUT_PULLUP)
	{
		*reg &= ~bit;
		*out |= bit;
	}
	else
	{
		*reg |= bit;
	}

	SREG = oldSREG;
}

void turnOffPWM(uint8_t timer)
{
	switch (timer)
	{
	case TIMER0:
		cbi(TCCR0A, COM01);
		break;
	case TIMER1A:
		cbi(TCCR1A, COM1A1);
		break;
	case TIMER1B:
		cbi(TCCR1A, COM1B1);
		break;
	}
}

void digitalWrite(uint8_t pin, uint8_t val)
{
	uint8_t timer = digitalPinToTimer(pin);
	uint8_t bit = digitalPinToBitMask(pin);
	uint8_t port = digitalPinToPort(pin);
	volatile uint8_t *out;

	if (port == NOT_A_PIN)
		return;

	// If the pin that support PWM output, we need to turn it off
	// before doing a digital write.
	if (timer != NOT_ON_TIMER)
		turnOffPWM(timer);

	out = portOutputRegister(port);

	uint8_t oldSREG = SREG;
	cli();

	if (val == LOW)
	{
		*out &= ~bit;
	}
	else
	{
		*out |= bit;
	}

	SREG = oldSREG;
}

boolean digitalRead(uint8_t pin)
{
	uint8_t timer = digitalPinToTimer(pin);
	uint8_t bit = digitalPinToBitMask(pin);
	uint8_t port = digitalPinToPort(pin);

	if (port == NOT_A_PIN)
		return LOW;
	if (timer != NOT_ON_TIMER)
		turnOffPWM(timer);

	volatile byte pv = *portInputRegister(port);

	return (pv & bit ? 0x01 : 0x00);
}
