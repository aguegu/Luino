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
}

void loop()
{
	int adc = analogRead(A7);

	usart.write(highByte(adc));
	usart.write(lowByte(adc));
	delay(500);
}
