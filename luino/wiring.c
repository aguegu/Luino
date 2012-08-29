/*
 * wiring.c
 *
 *  Created on: 2012-8-29
 *      Author: Agu
 */


#include "wiring_private.h"

void init()
{
	CLKPR = 0x80;
	cbi(CLKPR, CLKPCE);
	sei();
	sbi(MCUCR, JTD);
}



