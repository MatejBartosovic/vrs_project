/*
 * Spi1.h
 *
 *  Created on: Dec 10, 2016
 *      Author: Matejko
 */

#ifndef SPI1_H_
#define SPI1_H_


#include <SpiGeneric.h>

class Spi1 : public SpiGeneric {
public:
	//constructor
	Spi1();

	//init spi1
	void init();

	//destructor
	virtual ~Spi1();
};

#endif /* SPI1_H_ */
