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

#define TIMER_4BIT_WAVEFROM_NORMAL			0
#define TIMER_4BIT_WAVEFROM_PCPWM_8BIT		1
#define TIMER_4BIT_WAVEFROM_PCPWM_9BIT		2
#define TIMER_4BIT_WAVEFROM_PCPWM_10BIT		3
#define TIMER_4BIT_WAVEFROM_CTC_OCR1A		4
#define TIMER_4BIT_WAVEFROM_FASTPWM_8BIT	5
#define TIMER_4BIT_WAVEFROM_FASTPWM_9BIT	6
#define TIMER_4BIT_WAVEFROM_FASTPWM_10BIT	7
#define TIMER_4BIT_WAVEFROM_PFCPWM_ICR1		8
#define TIMER_4BIT_WAVEFROM_PFCPWM_OCR1A	9
#define TIMER_4BIT_WAVEFROM_PCPWM_ICR1		10
#define TIMER_4BIT_WAVEFROM_PCPWM_OCR1A		11
#define TIMER_4BIT_WAVEFROM_CTC_ICR1		12
#define TIMER_4BIT_WAVEFROM_FASTPWM_ICR1	14
#define TIMER_4BIT_WAVEFROM_FASTPWM_OCR1A	15

#define ADC_PRECSCALER_DIVISION_2	0
#define ADC_PRECSCALER_DIVISION_4	2
#define ADC_PRECSCALER_DIVISION_8	3
#define ADC_PRECSCALER_DIVISION_16	4
#define ADC_PRECSCALER_DIVISION_32	5
#define ADC_PRECSCALER_DIVISION_64	6
#define ADC_PRECSCALER_DIVISION_128	7

#define TIMER_CS(reg, n, clock_selection)	\
	dbi(reg, CS##n##2, bitRead(clock_selection, 2));	\
	dbi(reg, CS##n##1, bitRead(clock_selection, 1));	\
	dbi(reg, CS##n##0, bitRead(clock_selection, 0))

#define TIMER_3BIT_WAVEFORM(n, waveform)	\
	dbi(TCCR##n##B, WGM##n##2, bitRead(waveform, 2));	\
	dbi(TCCR##n##A, WGM##n##1, bitRead(waveform, 1));	\
	dbi(TCCR##n##A, WGM##n##0, bitRead(waveform, 0))

#define TIMER_4BIT_WAVEFORM(n, waveform)	\
	dbi(TCCR##n##B, WGM##n##3, bitRead(waveform, 3));	\
	dbi(TCCR##n##B, WGM##n##2, bitRead(waveform, 2));	\
	dbi(TCCR##n##A, WGM##n##1, bitRead(waveform, 1));	\
	dbi(TCCR##n##A, WGM##n##0, bitRead(waveform, 0))

#define ADC_PRESCALER(division)	\
	dbi(ADCSRA, ADPS2, bitRead(division, 2));	\
	dbi(ADCSRA, ADPS1, bitRead(division, 1));	\
	dbi(ADCSRA, ADPS0, bitRead(division, 0))

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

	TIMER_CS(TCCR1B, 1, TIMER_WITH_EXT_CLK_CS_64);
	TIMER_4BIT_WAVEFORM(1, TIMER_4BIT_WAVEFROM_PCPWM_8BIT);

	ADC_PRESCALER(ADC_PRECSCALER_DIVISION_128);
	sbi(ADCSRA, ADEN);

}



