// RHSPIDriver.cpp
//
// Copyright (C) 2014 Mike McCauley
// $Id: RHSPIDriver.cpp,v 1.9 2014/05/03 00:20:36 mikem Exp $

#include <RHSPIDriver.h>

RHSPIDriver::RHSPIDriver( GenericSPI& spi)
    :
    _spi(spi)
{
	_spi = spi;
}

bool RHSPIDriver::init()
{
    // start the SPI library with the default speeds etc:
    // On Arduino Due this defaults to SPI1 on the central group of 6 SPI pins
    _spi.begin();
    // Initialise the slave select pin
    // On Maple, this must be _after_ spi.begin

    usleep(100000);
    return true;
}

uint8_t RHSPIDriver::spiRead(uint8_t reg)
{

    char received[2];
    char send[] = {reg & ~RH_SPI_WRITE_MASK,0x00};
    _spi.transfernb(send,received,2); // Send the address with the write mask off
    return received[1];
}

uint8_t RHSPIDriver::spiWrite(uint8_t reg, uint8_t val)
{

    char received[2];
    char send[] = {reg | RH_SPI_WRITE_MASK,val};
    _spi.transfernb(send,received,2); // Send the address with the write mask on

    return received[0];
}

uint8_t RHSPIDriver::spiBurstRead(uint8_t reg, uint8_t* dest, uint8_t len)
{

    char received[len+1];
    char send[len+1];
    send[0] = reg & ~RH_SPI_WRITE_MASK;
    uint8_t status = 0;
    _spi.transfernb(send,received,len+1 ); // Send the start address with the write mask off
    return received[0];
}

uint8_t RHSPIDriver::spiBurstWrite(uint8_t reg, const uint8_t* src, uint8_t len)
{
	//TODO prerobit
    uint8_t status = 0;

    char received[len+1];
    char send[len+1];
    send[0] = reg | RH_SPI_WRITE_MASK;
    _spi.transfernb(send,received,len);

    return received[0];
}



