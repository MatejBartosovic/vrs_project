/*
 * Receiver.h
 *
 *  Created on: Jan 9, 2017
 *      Author: Matejko
 */

#ifndef RECEIVER_H_
#define RECEIVER_H_

#include <Rfm22.h>
#include <Timer.h>

class Receiver : public Rfm22 {
public:
	Receiver(SpiGeneric& spi);
	void irqHandler();
	void init();
	uint8_t* getValues();
	uint8_t getNumberOfValidMsgs();
	void resetNumberOfValidMsgs();
	virtual ~Receiver();
protected:
	Timer timer;
	//Timer safetyTimer;
private:
	uint8_t validMsgsReceived;

};

#endif /* RECEIVER_H_ */
