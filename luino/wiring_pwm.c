/*
 * wiring_analog.c
 *
 *  Created on: 2012-8-30
 *      Author: Agu
 */

#include "wiring_private.h"
#include "pins_luino.h"


void pwmWrite(uint8_t pin, uint8_t val)
{
	pinMode(pin, OUTPUT);
	if (val == 0)
	{
		digitalWrite(pin, LOW);
	}
	else if (val == 255)
	{
		digitalWrite(pin, HIGH);
	}
	else
	{
		switch(digitalPinToTimer(pin))
		{
			case TIMER0:
				sbi(TCCR0A, COM01);
				OCR0A = val; // set pwm duty
				break;
			case TIMER1A:
				sbi(TCCR1A, COM1A1);
				OCR1A = val; // set pwm duty
				break;
			case TIMER1B:
				sbi(TCCR1A, COM1B1);
				OCR1B = val; // set pwm duty
				break;
			case NOT_ON_TIMER:
			default:
				digitalWrite(pin, val & 0x80);
				break;
		}
	}
}
