/*
 * Luino.h
 *
 *  Created on: 2012-8-29
 *      Author: Agu
 */

#ifndef Luino_h_
#define Luino_h_

#include <stdlib.h>
#include <string.h>
#include <math.h>

#include <avr/pgmspace.h>
#include <avr/io.h>
#include "iolgt8f0xa.h"
#include <avr/interrupt.h>
#include <util/delay.h>

#ifdef __cplusplus
extern "C"{
#endif

// undefine stdlib's abs if encountered
#ifdef abs
#undef abs
#endif

#define min(a,b) ((a)<(b)?(a):(b))
#define max(a,b) ((a)>(b)?(a):(b))
#define abs(x) ((x)>0?(x):-(x))
#define sq(x) ((x)*(x))

#define lowByte(w) ((uint8_t) ((w) & 0xff))
#define highByte(w) ((uint8_t) ((w) >> 8))

#define bitRead(value, bit) (((value) >> (bit)) & 0x01)
#define bitSet(value, bit) ((value) |= _BV(bit))
#define bitClear(value, bit) ((value) &= ~_BV(bit))
#define bitWrite(value, bit, bitvalue) (bitvalue ? bitSet(value, bit) : bitClear(value, bit))

typedef unsigned int word;
typedef uint8_t boolean;
typedef uint8_t byte;

extern const uint8_t PROGMEM digital_pin_to_port_PGM[];
extern const uint8_t PROGMEM digital_pin_to_bit_mask_PGM[];

#define digitalPinToPort(P) ( pgm_read_byte( digital_pin_to_port_PGM + (P) ) )
#define digitalPinToBitMask(P) ( pgm_read_byte( digital_pin_to_bit_mask_PGM + (P) ) )

extern const uint16_t PROGMEM port_to_mode_PGM[];
extern const uint16_t PROGMEM port_to_input_PGM[];
extern const uint16_t PROGMEM port_to_output_PGM[];

#define portOutputRegister(P) ( (volatile uint8_t *)( pgm_read_word( port_to_output_PGM + (P))) )
#define portInputRegister(P) ( (volatile uint8_t *)( pgm_read_word( port_to_input_PGM + (P))) )
#define portModeRegister(P) ( (volatile uint8_t *)( pgm_read_word( port_to_mode_PGM + (P))) )

#define INPUT 0x0
#define OUTPUT 0x1
#define INPUT_PULLUP 0x2

#define NOT_A_PIN 0
#define NOT_A_PORT 0

#ifdef LUINO_MAIN
#define PORT_A 1
#define PORT_B 2
#define PORT_C 3
#define PORT_D 4
#endif

void init(void);
void setup(void);
void loop(void);

unsigned long millis();
unsigned long micros();
void delay(unsigned long ms);
void delayMicroseconds(unsigned int us);

void pinMode(uint8_t, uint8_t);



#ifndef PIN_WRITE
#define pinSet(pin)	*portOutputRegister(digitalPinToPort(pin)) |= digitalPinToBitMask(pin)
#define pinClear(pin)	*portOutputRegister(digitalPinToPort(pin)) &= ~digitalPinToBitMask(pin)
#define pinWrite(pin, val) ( (val) ? pinSet(pin) : pinClear(pin))

#endif


#ifdef __cplusplus
} // extern "C"
#endif

#ifdef __cplusplus
#include "wiring_private.h"
#include "Usart.h"

#endif

#include "pins_luino.h"

#endif /* LUINO_H_ */
