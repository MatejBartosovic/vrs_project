/*
 * RHSPIDriver.h
 *
 *  Created on: 4 Dec 2016
 *      Author: pi
 */

#ifndef RHSPIDRIVER_H_
#define RHSPIDRIVER_H_

#include "RaspberryPiSPI.h"
#include "RHGenericDriver.h"
#include <inttypes.h>

#define RH_SPI_WRITE_MASK 0x80

class RHSPIDriver : public RHGenericDriver {
public:
	RHSPIDriver(GenericSPI& spi);
	virtual ~RHSPIDriver();

	bool init();

	uint8_t spiRead(uint8_t reg);
	uint8_t spiWrite(uint8_t reg, uint8_t val);
	uint8_t spiBurstRead(uint8_t reg, uint8_t* dest, uint8_t len);
	uint8_t spiBurstWrite(uint8_t reg, const uint8_t* src, uint8_t len);

	bool available(){return false;};

	bool recv(uint8_t* buf, uint8_t* len){return false;};
	bool send(const uint8_t* data, uint8_t len){return false;};
	uint8_t maxMessageLength(){return 10;};
private:
protected:
	GenericSPI&_spi;
};

#endif /* RHSPIDRIVER_H_ */
