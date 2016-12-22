/*
 * Transmitter.cpp
 *
 *  Created on: Dec 22, 2016
 *      Author: Matejko
 */

#include <Transmitter.h>
#include <JoystickGeneric.h>

Transmitter::Transmitter(SpiGeneric &spi,JoystickGeneric &joy) : Rfm22(spi), joy(joy) {
	// TODO Auto-generated constructor stub
}

void Transmitter::init() {
	Rfm22::init();
}

Transmitter::~Transmitter() {
	// TODO Auto-generated destructor stub
}

