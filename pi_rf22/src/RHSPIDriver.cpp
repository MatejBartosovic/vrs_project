/*
 * RHSPIDriver.cpp
 *
 *  Created on: 4 Dec 2016
 *      Author: pi
 */

#include "RHSPIDriver.h"
#include <string.h>

RHSPIDriver::RHSPIDriver(GenericSPI &spi) : _spi(spi){
	// TODO Auto-generated constructor stub

}
bool RHSPIDriver::init(){
	//TODO init
	_spi.begin();
    return true;
}

uint8_t RHSPIDriver::spiRead(uint8_t reg){
	char send[2];
	char receive[2];
	send[0] = (char)reg & ~RH_SPI_WRITE_MASK;
	_spi.transfernb(send,receive,2);
	return receive[1];
}

uint8_t RHSPIDriver::spiWrite(uint8_t reg, uint8_t val){
	uint8_t status = 0;
	char send[2];
	char receive[2];
	send[0] = (char)reg | RH_SPI_WRITE_MASK;
	_spi.transfernb(send,receive,2);
	return status;
}

uint8_t RHSPIDriver::spiBurstRead(uint8_t reg, uint8_t* dest, uint8_t len){
	char send[len+1];
	char receive[len+1];
	send[0] = (char)reg & ~RH_SPI_WRITE_MASK;
	_spi.transfernb(send,receive,len+1);
	memcpy(dest,(receive+1),len*sizeof(char));
	return receive[0];
}

uint8_t RHSPIDriver::spiBurstWrite(uint8_t reg, const uint8_t* src, uint8_t len){
	char send[len+1];
	char receive[len+1];
	memcpy(send+1,src,len*sizeof(char));
	send[0] = (char)reg | RH_SPI_WRITE_MASK;
	_spi.transfernb(send,receive,len+1);
	return receive[0];
}

RHSPIDriver::~RHSPIDriver() {
	// TODO Auto-generated destructor stub
}

