/*
 * Usart.h
 *
 *  Created on: 2012-8-26
 *      Author: Agu
 */

#ifndef USART_H_
#define USART_H_

#include <inttypes.h>
#include "new.h"
#include <avr/sfr_defs.h>
#include <avr/interrupt.h>
#include "Luino.h"


#ifndef cbi
#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#endif
#ifndef sbi
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#endif

struct ring_buff
{
	uint8_t *buffer;
	volatile uint8_t index_write;	// to receive the incoming byte
	volatile uint8_t index_read;	// to send byte
};

class Usart
{
public:
	Usart(volatile uint8_t *ubrrh, volatile uint8_t *ubrrl,
			volatile uint8_t *ucsra, volatile uint8_t *ucsrb,
			volatile uint8_t *udr, uint8_t rxen, uint8_t txen, uint8_t rxcie,
			uint8_t udrie, uint8_t u2x, uint8_t buff_size);
	virtual ~Usart();
	void begin(uint32_t baudrate);
	void end();
	void write(uint8_t c);
	void transmit();
	void receive();
	uint8_t available(void);
	uint8_t peek(void);
	uint8_t read(void);
	void flush(void);
private:
	volatile uint8_t * const _ubrrh;
	volatile uint8_t * const _ubrrl;
	volatile uint8_t * const _ucsra;
	volatile uint8_t * const _ucsrb;
	volatile uint8_t * const _udr;
	const uint8_t _rxen;
	const uint8_t _txen;
	const uint8_t _rxcie;
	const uint8_t _udrie;
	const uint8_t _u2x;
	const uint8_t _buff_size;

	ring_buff _tx_buff;
	ring_buff _rx_buff;
};

extern Usart usart;

#endif /* USART_H_ */


