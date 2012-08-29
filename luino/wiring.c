/*
 * wiring.c
 *
 *  Created on: 2012-8-29
 *      Author: Agu
 */


#include "wiring_private.h"

void init()
{
	sei();
	sbi(MCUCR, JTD);
}



