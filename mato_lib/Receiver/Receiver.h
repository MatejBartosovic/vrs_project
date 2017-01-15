/*
 * Receiver.h
 *
 *  Created on: Jan 9, 2017
 *      Author: Matejko
 */

#ifndef RECEIVER_H_
#define RECEIVER_H_

/*
 * Class which reconfigure rfm_22 to behave as receiver
 * Each new message is written to first byte of _buf, old data are overwritten
 * Timer periodically set data from _buf to pwm registers
 * SafetyTimer periodically check number of valid received messages and stop motors when no message is received
 * */

#include <Rfm22.h>
#include <Timer.h>

class Receiver : public Rfm22 {
public:
	//constructor
	//SpiGeneric - pointer to class which inherit from SpiGeneric class
	Receiver(SpiGeneric& spi);

	//irqHandler had to be called on interrupt
	void irqHandler();

	//init receiver
	void init();

	//get values which will be set to motors
	uint8_t* getValues();

	//return number of valid messages
	uint8_t getNumberOfValidMsgs();

	//reset vallid messages counter
	void resetNumberOfValidMsgs();

	//destructor
	virtual ~Receiver();
protected:
	Timer timer;
	Timer safetyTimer;
private:
	uint8_t validMsgsReceived;

};

#endif /* RECEIVR_H_ */
