/*
 * Transmitter.h
 *
 *  Created on: Dec 22, 2016
 *      Author: Matejko
 */

#ifndef TRANSMITTER_H_
#define TRANSMITTER_H_

#include <rfm22.h>
#include <SpiGeneric.h>
#include <JoystickGeneric.h>


class Transmitter : private Rfm22 {
public:
	Transmitter(SpiGeneric &spi, JoystickGeneric &joy);
	void init();
	virtual ~Transmitter();
private:
protected:
	JoystickGeneric &joy;
};

#endif /* TRANSMITTER_H_ */
