//============================================================================
// Name        : pi_rf22.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <stdio.h>
#include "RaspberryPiSPI.h"
#include "RH_RF22.h"
#include <unistd.h>

int main() {
	char data[]= {0x00,0x00};
	char send[] = {0x02 ,0x00};
	if(!bcm2835_init()){
		printf("bcm init failed");
	}
	RaspberryPiSPI spi(BCM2835_SPI_CS0, BCM2835_SPI_CLOCK_DIVIDER_4096, BCM2835_SPI_BIT_ORDER_MSBFIRST,  BCM2835_SPI_MODE0);
	spi.begin();
	spi.transfernb(send,data,2);
	printf("received:  %x %x \n",data[0],data[1]);
	RH_RF22 a(2,spi);

	sleep(1);
	printf("received:  %x \n",a.statusRead());

	return 0;
}

