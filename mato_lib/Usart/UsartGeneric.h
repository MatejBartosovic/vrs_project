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
	UsartGeneric();
	virtual void init() = 0;
	virtual uint8_t read(uint8_t* data,uint8_t len);
	virtual void write(uint8_t* data,uint8_t len);
	virtual uint8_t availableBytes();
	virtual void flush();
	virtual uint8_t writableBytes();
	virtual void irqHandler (void);
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
