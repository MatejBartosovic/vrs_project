/*
 * RaspberryPiSPI.cpp
 *
 *  Created on: 4 Dec 2016
 *      Author: pi
 */

#include"RaspberryPiSPI.h"
#include <stdio.h>
#include <unistd.h>

void RaspberryPiSPI::begin(){

	if(!bcm2835_spi_begin()){
		printf("begin failed");
	}
	bcm2835_spi_setChipSelectPolarity(cs,0);
    bcm2835_spi_setClockDivider(divider);
	bcm2835_spi_setDataMode(dataMode);
	bcm2835_spi_chipSelect(cs);
    bcm2835_spi_setBitOrder(bitOrder);
    return ;
};

void RaspberryPiSPI::end(){
    bcm2835_spi_end();
};

uint8_t RaspberryPiSPI::transfer(uint8_t data){
	return bcm2835_spi_transfer(data);
};

void RaspberryPiSPI::transfernb(char* tbuf, char* rbuf,uint32_t len){
	bcm2835_spi_transfernb(tbuf,rbuf,len);
}

RaspberryPiSPI::~RaspberryPiSPI(){
		end();
	};

