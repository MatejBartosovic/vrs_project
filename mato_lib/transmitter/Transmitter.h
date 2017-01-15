/*
 * Transmitter.h
 *
 *  Created on: Dec 22, 2016
 *      Author: Matejko
 */

#ifndef TRANSMITTER_H_
#define TRANSMITTER_H_

/*
 * Class which use rfm_22 to periodically send data
 * Timer is used to periodical broadcasting.
 * */

#include <rfm22.h>
#include <SpiGeneric.h>
#include <JoystickGeneric.h>
#include <Timer.h>

class Transmitter : public Rfm22 {
public:
	//constructor
	//SpiGeneric - pointer to class which inherit from SpiGeneric class
	Transmitter(SpiGeneric& spi );

	//init transmitter
	void init();

	//destructor
	virtual ~Transmitter();
private:
	Timer timer;
protected:
};

#endif /* TRANSMITTER_H_ */
