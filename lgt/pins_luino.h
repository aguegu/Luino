/*
 * pins_luino.h
 *
 *  Created on: 2012-8-29
 *      Author: Agu
 */

#ifndef PINS_LUINO_H_
#define PINS_LUINO_H_

#include <avr/pgmspace.h>

#ifdef LUINO_MAIN

// LGT8
//
//                     +-\/-+
//  T0  | D13 |  PB0  1|    |28  PB1  | D14 | T1
// XTAL | D8  |  PC0  2|    |27  RST
// XTAL | D9  |  PC1  3|    |26  PD2  | D2  | INT0
//               GND  4|    |25  PD1  | D1  | TXD
//  OC2 | D12 |  PC6  5|    |24  PD0  | D0  | RXD
//               VCC  6|    |23  PD3  | D3  | INT1
//  A0  | D17 |  PA0  7|    |22  PD4  | D4  | OC1B
//  A1  | D18 |  PA1  8|    |21  PD5  | D5  | OC1A
//  A2  | D19 |  PA2  9|    |20  PB2  | D15 | LED  | SCL
//  A3  | D20 |  PA3 10|    |19  PB3  | D16
//  A4  | D21 |  PA4 11|    |18  PC3  | D11
//  A5  | D22 |  PA5 12|    |17  PC2  | D10
//  A6  | D23 |  PA6 13|    |16  PD6  | D6
//  A7  | D24 |  PA7 14|    |15  PD7  | D7  | INT2
//                     +----+
//
// these arrays map port names (e.g. port B) to the
// appropriate addresses for various functions (e.g. reading
// and writing)
const uint16_t PROGMEM port_to_mode_PGM[] =
{
	NOT_A_PORT,
	(uint16_t) &DDRA,
	(uint16_t) &DDRB,
	(uint16_t) &DDRC,
	(uint16_t) &DDRD,
};

const uint16_t PROGMEM port_to_output_PGM[] =
{
	NOT_A_PORT,
	(uint16_t) &PORTA,
	(uint16_t) &PORTB,
	(uint16_t) &PORTC,
	(uint16_t) &PORTD,
};

const uint16_t PROGMEM port_to_input_PGM[] =
{
	NOT_A_PORT,
	(uint16_t) &PINA,
	(uint16_t) &PINB,
	(uint16_t) &PINC,
	(uint16_t) &PIND,
};

const uint8_t PROGMEM digital_pin_to_port_PGM[] =
{
	PORT_D, PORT_D, PORT_D, PORT_D, PORT_D, PORT_D, PORT_D, PORT_D,
	PORT_C, PORT_C, PORT_C, PORT_C, PORT_C,
	PORT_B, PORT_B, PORT_B, PORT_B,
	PORT_A, PORT_A, PORT_A, PORT_A, PORT_A, PORT_A, PORT_A, PORT_A,
};

const uint8_t PROGMEM digital_pin_to_bit_mask_PGM[] =
{
	_BV(0), _BV(1), _BV(2), _BV(3), _BV(4), _BV(5), _BV(6), _BV(7),
	_BV(0), _BV(1), _BV(2), _BV(3), _BV(6),
	_BV(0), _BV(1), _BV(2), _BV(3),
	_BV(0), _BV(1), _BV(2), _BV(3), _BV(4), _BV(5), _BV(6), _BV(7),
};

#endif

#endif /* PINS_LUINO_H_ */