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
#include <Timer.h>

class Transmitter : public Rfm22 {
public:
	Transmitter(SpiGeneric& spi );
	void init();
	virtual ~Transmitter();
private:
	void initTimer();
	Timer timer;
protected:
};

#endif /* TRANSMITTER_H_ */
