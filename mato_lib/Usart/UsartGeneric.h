/*
 * UsartGeneric.h
 *
 *  Created on: Dec 8, 2016
 *      Author: Matejko
 */

#ifndef USARTGENERIC_H_
#define USARTGENERIC_H_

#include <stddef.h>
#include "stm32l1xx.h"

class UsartGeneric {
public:
	//constructor
	UsartGeneric();

	//this method must be implemented in inherited class
	virtual void init() = 0;

	//read len bytes to data, number of red bytes is returned
	virtual uint8_t read(uint8_t* data,uint8_t len);

	//write len bytes to usart
	virtual void write(uint8_t* data,uint8_t len);

	//return available bytes in receive buffer
	virtual uint8_t availableBytes();

	//flush receive buffer
	virtual void flush();

	//return numer of bytes waiting to be send
	virtual uint8_t writableBytes();

	//irq handler needed to be called when interupt occur
	virtual void irqHandler (void);

	//destructor
	virtual ~UsartGeneric();
private:
	void initWrite(uint8_t new_bytes);

protected:
	USART_TypeDef* usart;
	volatile uint8_t usart_send_it;
	volatile uint8_t usart_write_buffer_it;
	volatile uint8_t usart_write_buffer[256];

	volatile uint8_t usart_read_it;
	volatile uint8_t usart_read_buffer_it;
	volatile uint8_t usart_read_buffer[256];

};

#endif /* USARTGENERIC_H_ */
