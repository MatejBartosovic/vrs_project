/*
 * SpiGeneric.h
 *
 *  Created on: Dec 10, 2016
 *      Author: Matejko
 */

#ifndef SPIGENERIC_H_
#define SPIGENERIC_H_
#define SPI_WRITE_FLAG 0x80


/*
 * Template class to spi interface.
 * This class offer basic read and write function which can be reimplemented.
 * Function init need to be implemented by inherited class.
 * */

#include <stddef.h>
#include "stm32l1xx.h"

class SpiGeneric {

public:
	//constructor
	SpiGeneric();

	//this method must be implemented in inherited class
	virtual void init() = 0;

	//write len bytes from buf to reg address
	void writeRegBytes(uint8_t reg,uint8_t* buf,uint8_t len);

	//write one byte to register
	void writeReg(uint8_t reg,uint8_t val);

	//read reg and return value
	uint8_t readReg(uint8_t reg);

	//read len bytes from register to buf
	void readRegBytes(uint8_t reg,uint8_t* buf,uint8_t len);

	//write len bytes from buf to spi bus and fill buffer with red values
	void writeRead(uint8_t* buf,uint8_t len);

	//destructor
	virtual ~SpiGeneric();

private:

protected:
	SPI_TypeDef* spi;
	uint16_t cs;
	GPIO_TypeDef* gpio_cs;
	inline void waitTxe();
	inline void waitRxne();
	inline void waitTxRx();
};

#endif /* SPIGENERIC_H_ */
