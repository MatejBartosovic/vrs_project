/*
 * RaspberryPiSPI.h
 *
 *  Created on: 3 Dec 2016
 *      Author: pi
 */

#ifndef RASPBERRYPISPI_H_
#define RASPBERRYPISPI_H_
#include <bcm2835.h>
#include "GenericSPI.h"

class RaspberryPiSPI : public GenericSPI{

	public:
	RaspberryPiSPI(bcm2835SPIChipSelect cs = BCM2835_SPI_CS0, bcm2835SPIClockDivider divider = BCM2835_SPI_CLOCK_DIVIDER_16, bcm2835SPIBitOrder bitOrder = BCM2835_SPI_BIT_ORDER_MSBFIRST, bcm2835SPIMode dataMode = BCM2835_SPI_MODE0) : GenericSPI(Frequency1MHz,BitOrderMSBFirst,DataMode0){
		this->cs = cs;
		this->divider = divider;
		this->bitOrder = bitOrder;
		this->dataMode = dataMode;
	}

	uint8_t transfer(uint8_t data);

	void transfernb(char* tbuf, char* rbuf,uint32_t len);

    /*void attachInterrupt(){};
    void detachInterrupt(){};*/

    void begin();
    void end();

	virtual ~RaspberryPiSPI();
protected:
	bcm2835SPIClockDivider divider;
	bcm2835SPIBitOrder bitOrder;
	bcm2835SPIMode dataMode;
	bcm2835SPIChipSelect cs;
};

#endif /* RASPBERRYPISPI_H_ */
