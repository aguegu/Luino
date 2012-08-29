/*
 * Usart.cpp
 *
 *  Created on: 2012-8-26
 *      Author: Agu
 */

#include "Usart.h"

Usart::Usart(volatile uint8_t *ubrrh, volatile uint8_t *ubrrl,
		volatile uint8_t *ucsra, volatile uint8_t *ucsrb, volatile uint8_t *udr,
		uint8_t rxen, uint8_t txen, uint8_t rxcie, uint8_t udrie, uint8_t u2x,
		uint8_t buff_size) :
		_ubrrh(ubrrh), _ubrrl(ubrrl), _ucsra(ucsra), _ucsrb(ucsrb), _udr(udr), _rxen(
				rxen), _txen(txen), _rxcie(rxcie), _udrie(udrie), _u2x(u2x), _buff_size(
				buff_size)
{
	// TODO Auto-generated constructor stub
	_tx_buff.index_write = 0;
	_tx_buff.index_read = 0;
	_tx_buff.buffer = (uint8_t *) malloc(sizeof(uint8_t) * _buff_size);

	_rx_buff.index_write = 0;
	_rx_buff.index_read = 0;
	_rx_buff.buffer = (uint8_t *) malloc(sizeof(uint8_t) * _buff_size);
}

Usart::~Usart()
{
	free(_tx_buff.buffer);
	free(_rx_buff.buffer);
}

void Usart::begin(uint32_t baudrate)
{
	uint16_t baud_setting;
	sbi(*_ucsra, _u2x);

	baud_setting = (F_CPU / 4 / baudrate - 1) / 2;

	if (baud_setting > 4095)
	{
		cbi(*_ucsra, _u2x);
		baud_setting = (F_CPU / 8 / baudrate - 1) / 2;
	}

	// assign the baud_setting, a.k.a. ubbr (USART Baud Rate Register)
	*_ubrrh = baud_setting >> 8;
	*_ubrrl = baud_setting;

	sbi(*_ucsrb, _rxen);
	sbi(*_ucsrb, _txen);
	sbi(*_ucsrb, _rxcie);
	cbi(*_ucsrb, _udrie);
}

void Usart::end()
{
	while (_tx_buff.index_write != _tx_buff.index_read)
		;

	cbi(*_ucsrb, _rxen);
	cbi(*_ucsrb, _txen);
	cbi(*_ucsrb, _rxcie);
	cbi(*_ucsrb, _udrie);
}

void Usart::write(uint8_t c)
{
	uint8_t i = (_tx_buff.index_write + 1) % _buff_size;

	while (i == _tx_buff.index_read)
		;

	_tx_buff.buffer[_tx_buff.index_write] = c;
	_tx_buff.index_write = i;

	sbi(*_ucsrb, _udrie);
}

void Usart::transmit()
{
	if (_tx_buff.index_write == _tx_buff.index_read)
	{
		cbi(*_ucsrb, _udrie);
		// Buffer empty, so disable interrupts
	}
	else
	{
		// There is more data in the output buffer. Send the next byte
		uint8_t c = _tx_buff.buffer[_tx_buff.index_read];
		_tx_buff.index_read = (_tx_buff.index_read + 1) % _buff_size;

		*_udr = c;
	}
}

void Usart::receive()
{
	uint8_t i = (_rx_buff.index_write + 1) % _buff_size;

	// if we should be storing the received character into the location
	// just before the tail (meaning that the head would advance to the
	// current location of the tail), we're about to overflow the buffer
	// and so we don't write the character or advance the head.
	if (i != _rx_buff.index_read)
	{
		_rx_buff.buffer[_rx_buff.index_write] = *_udr;
		_rx_buff.index_write = i;
	}
}

uint8_t Usart::available(void)
{
	return (_buff_size + _rx_buff.index_write - _rx_buff.index_read)
			% _buff_size;
}

uint8_t Usart::peek(void)
{
	if (_rx_buff.index_write == _rx_buff.index_read)
	{
		return -1;
	}
	else
	{
		return _rx_buff.buffer[_rx_buff.index_read];
	}
}

uint8_t Usart::read(void)
{
	if (_rx_buff.index_write == _rx_buff.index_read)
	{
		return -1;
	}
	else
	{
		uint8_t c = _rx_buff.buffer[_rx_buff.index_read];
		_rx_buff.index_read = (_rx_buff.index_read + 1) % _buff_size;
		return c;
	}
}

Usart usart(&UBRRH, &UBRRL, &UCSRA, &UCSRB, &UDR, RXEN, TXEN, RXCIE, UDRIE, U2X,
		16);

ISR(USART_UDRE_vect)
{
	usart.transmit();
}

ISR(USART_RXC_vect)
{
	usart.receive();
}
