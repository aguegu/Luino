/*
 * wiring.c
 *
 *  Created on: 2012-8-29
 *      Author: Agu
 */
#include "wiring_private.h"

#define SCALE_US_PER_TIMER0_COUNT 2
#define TIMER_COUNT_PER_MS	250

#define TIMER_WITH_EXT_CLK_CS_NUL	0
#define TIMER_WITH_EXT_CLK_CS_1		1
#define TIMER_WITH_EXT_CLK_CS_8		2
#define TIMER_WITH_EXT_CLK_CS_64	3
#define TIMER_WITH_EXT_CLK_CS_256	4
#define TIMER_WITH_EXT_CLK_CS_1024	5
#define TIMER_WITH_EXT_CLK_CS_EXT_FALL	6
#define TIMER_WITH_EXT_CLK_CS_EXT_RISE	7

#define TIMER_3BIT_WAVEFORM_NORMAL	0
#define TIMER_3BIT_WAVEFORM_PCPWM	1
#define TIMER_3BIT_WAVEFORM_CTC		2
#define TIMER_3BIT_WAVEFORM_FPWM	3
#define TIMER_3BIT_WAVEFORM_PCPWM_OCRA	5
#define TIMER_3BIT_WAVEFORM_FPWM_OCRA	7

#define TIMER_CS(reg, n, clock_selection)	\
	dbi(reg, CS##n##2, bitRead(clock_selection, 2));	\
	dbi(reg, CS##n##1, bitRead(clock_selection, 1));	\
	dbi(reg, CS##n##0, bitRead(clock_selection, 0))

#define TIMER_3BIT_WAVEFORM(n, waveform)	\
	dbi(TCCR##n##B, WGM##n##2, bitRead(waveform, 2));	\
	dbi(TCCR##n##A, WGM##n##1, bitRead(waveform, 1));	\
	dbi(TCCR##n##A, WGM##n##0, bitRead(waveform, 0))

volatile unsigned long timer0_overflow_count = 0;

ISR(TIMER0_OVF_vect)
{
	timer0_overflow_count++;
}

unsigned long millis()
{
	return timer0_overflow_count;
}

inline unsigned long time_stamp()
{
	uint8_t oldSREG = SREG;
	cli();
	unsigned long tcnt = TCNT0;
	SREG = oldSREG;
	return (tcnt + (timer0_overflow_count << 8));
}

unsigned long micros()
{
	unsigned long m = time_stamp() << SCALE_US_PER_TIMER0_COUNT;
	return m;
}

void delay(unsigned long ms)
{
	uint32_t due = timer0_overflow_count + ms;
	while (timer0_overflow_count<due);
}

void init()
{
//	cbi(PORTC, PC0);
//	cbi(PORTC, PC1);
//	PMCR = 0x34;

	CLKPR = 0x80;
	cbi(CLKPR, CLKPCE);

	sbi(MCUCR, JTD);
	sbi(MCUCR, JTD);

	sei();

	TIMER_CS(TCCR0B, 0, TIMER_WITH_EXT_CLK_CS_64);
	TIMER_3BIT_WAVEFORM(0, TIMER_3BIT_WAVEFORM_FPWM);

	sbi(TIMSK0, TOIE0);
}

