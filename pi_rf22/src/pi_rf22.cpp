//============================================================================
// Name        : pi_rf22.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <stdio.h>
#include "RaspberryPiSPI.h"
using namespace std;

int main() {
	char data[]= {0x00,0x00};
	char send[] = {0x01 ,0x00};
	if(!bcm2835_init()){
		printf("bcm init failed");
	}
	RaspberryPiSPI spi(BCM2835_SPI_CS0, BCM2835_SPI_CLOCK_DIVIDER_4096, BCM2835_SPI_BIT_ORDER_MSBFIRST,  BCM2835_SPI_MODE0);
	spi.begin();
	spi.transfernb(send,data,2);
	//data[0] = spi.transfer(0x01);
	//data[1] = spi.transfer(0x00);
	printf("received:  %x %x \n",data[0],data[1]);
	return 0;
}

