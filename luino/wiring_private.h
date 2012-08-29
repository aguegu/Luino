/*
 * wiring_private.h
 *
 *  Created on: 2012-8-29
 *      Author: Agu
 */

#ifndef WIRING_PRIVATE_H_
#define WIRING_PRIVATE_H_

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <stdarg.h>
#include <util/delay_basic.h>
#include "Luino.h"

#ifdef __cplusplus
extern "C"
{
#endif

#ifndef cbi
#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#endif

#ifndef sbi
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#endif

#ifndef dbi
#define dbi(sfr, bit, val) (val)?sbi(sfr, bit):cbi(sfr, bit)
#endif

typedef void (*voidFuncPtr)(void);

#ifdef __cplusplus
} // extern "C"
#endif


#endif /* WIRING_PRIVATE_H_ */
