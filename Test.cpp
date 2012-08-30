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
}

void loop()
{
	static byte val = 0x01;

	//digitalWrite(15, val);
	//val = !val;

	analogWrite(12, val);
	analogWrite(4, val);
	analogWrite(5, val);

	val++;

	delay(10);

//	uint32_t t = micros();
//
//	usart.write(t>>24);
//	usart.write(t>>16);
//	usart.write(t>>8);
//	usart.write(t);

}
