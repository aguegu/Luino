#include "wiring_private.h"
#include "pins_luino.h"

void analogReference(uint8_t mode)
{
	cbi(ADMUX, REFS0);
	dbi(ADMUX, REFS1, mode == REF_1V25);
}

int analogRead(uint8_t pin)
{
	uint8_t low, high;

	pin = analogPinToChannel(pin);

	cbi(ADMUX, ADPS0);
	cbi(ADMUX, ADPS1);
	cbi(ADMUX, ADPS2);
	ADMUX |= (pin & 0x07);

	sbi(ADCSRA, ADSC);

	loop_until_bit_is_set(ADCSRA, ADSC);

	low  = ADCL;
	high = ADCH;
	return (high << 8) | low;
}
