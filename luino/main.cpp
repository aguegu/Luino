/*
 * main.cpp
 *
 *  Created on: 2012-8-29
 *      Author: Agu
 */

#include "Luino.h"

int main(void)
{
	init();

	setup();

	for (;;)
	{
		loop();
	}

	return 0;
}


