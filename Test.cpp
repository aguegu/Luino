/*
 * Test.cpp
 *
 *  Created on: 2012-8-29
 *      Author: Agu
 */

#include "Test.h"

void setup()
{
	for (byte i=0;i<25;i++)
		pinMode(i, OUTPUT);
}

void loop()
{
	for (byte i=0;i<25;i++)
		pinSet(i);
	_delay_ms(100);
	for (byte i=0;i<25;i++)
		pinClear(i);
	_delay_ms(100);
}
