/*
 * Transmitter.cpp
 *
 *  Created on: Dec 22, 2016
 *      Author: Matejko
 */

#include <Transmitter.h>

Transmitter::Transmitter(SpiGeneric &spi) : Rfm22(spi), timer(TIM2, 16000, 100){
	// TODO Auto-generated constructor stub
}

void Transmitter::init() {
	Rfm22::init();
	timer.init();
	timer.start();
}

Transmitter::~Transmitter() {
	// TODO Auto-generated destructor stub
}

