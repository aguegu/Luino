/*
 * Test.cpp
 *
 *  Created on: 2012-8-29
 *      Author: Agu
 */

#include "Test.h"

void setup()
{
	usart.begin(9600);
	pinMode(15, OUTPUT);
	analogReference(REF_1V25);
	pinMode(A7, INPUT);
}

void loop()
{
	uint8_t val = digitalRead(A7);

	usart.write(val);
	delay(100);
}
