/*
 * SpiGeneric.h
 *
 *  Created on: Dec 10, 2016
 *      Author: Matejko
 */

#ifndef SPIGENERIC_H_
#define SPIGENERIC_H_
#define SPI_WRITE_FLAG 0x80


#include <stddef.h>
#include "stm32l1xx.h"

class SpiGeneric {

public:
	SpiGeneric();
	virtual void init() = 0;
	void writeRegBytes(uint8_t reg,uint8_t* buf,uint8_t len);
	void writeReg(uint8_t reg,uint8_t val);
	uint8_t readReg(uint8_t reg);
	void readRegBytes(uint8_t reg,uint8_t* buf,uint8_t len);
	void writeRead(uint8_t* buf,uint8_t len);
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
