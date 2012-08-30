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

/* Delay for the given number of microseconds.  Assumes a 8 or 16 MHz clock. */
void delayMicroseconds(unsigned int us)
{
	// calling avrlib's delay_us() function with low values (e.g. 1 or
	// 2 microseconds) gives delays longer than desired.
	//delay_us(us);
#if F_CPU >= 20000000L
	// for the 20 MHz clock on rare Arduino boards

	// for a one-microsecond delay, simply wait 2 cycle and return. The overhead
	// of the function call yields a delay of exactly a one microsecond.
	__asm__ __volatile__ (
			"nop" "\n\t"
			"nop");//just waiting 2 cycle
	if (--us == 0)
	return;

	// the following loop takes a 1/5 of a microsecond (4 cycles)
	// per iteration, so execute it five times for each microsecond of
	// delay requested.
	us = (us<<2) + us;// x5 us

	// account for the time taken in the preceeding commands.
	us -= 2;

#elif F_CPU >= 16000000L
	// for the 16 MHz clock on most Arduino boards

	// for a one-microsecond delay, simply return.  the overhead
	// of the function call yields a delay of approximately 1 1/8 us.
	if (--us == 0)
		return;

	// the following loop takes a quarter of a microsecond (4 cycles)
	// per iteration, so execute it four times for each microsecond of
	// delay requested.
	us <<= 2;

	// account for the time taken in the preceeding commands.
	us -= 2;
#else
	// for the 8 MHz internal clock on the ATmega168

	// for a one- or two-microsecond delay, simply return.  the overhead of
	// the function calls takes more than two microseconds.  can't just
	// subtract two, since us is unsigned; we'd overflow.
	if (--us == 0)
	return;
	if (--us == 0)
	return;

	// the following loop takes half of a microsecond (4 cycles)
	// per iteration, so execute it twice for each microsecond of
	// delay requested.
	us <<= 1;

	// partially compensate for the time taken by the preceeding commands.
	// we can't subtract any more than this or we'd overflow w/ small delays.
	us--;
#endif

	// busy wait
//	__asm__ __volatile__ (
//			"1: sbiw %0,1" "\n\t" // 2 cycles
//			"brne 1b" : "=w" (us) : "0" (us)// 2 cycles
//	);
	_delay_loop_2(us);
	// W.H. Guan: it is asm function is already defined in <util/delay_basic.h>
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

