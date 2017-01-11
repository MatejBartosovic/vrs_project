/*
 * Receiver.cpp
 *
 *  Created on: Jan 9, 2017
 *      Author: Matejko
 */

#include <Receiver.h>

Receiver::Receiver(SpiGeneric& spi) : Rfm22(spi), timer(TIM2, 16000, 100){
	// TODO Auto-generated constructor stub

}

void Receiver::irqHandler(){

    // Read the interrupt flags which clears the interrupt
    spi.readRegBytes(RH_RF22_REG_03_INTERRUPT_STATUS1, _lastInterruptFlags, 2);

    if (_lastInterruptFlags[0] & RH_RF22_IFFERROR){
    	resetFifos(); // Clears the interrupt
    	if (currentMode == Rfm22ModeTx )
    	    restartTransmit();
    	else if (currentMode == Rfm22ModeRx)
    	    clearRxBuf();
    }
        // Caution, any delay here may cause a FF underflow or overflow
    if (_lastInterruptFlags[0] & RH_RF22_ITXFFAEM){
        	// See if more data has to be loaded into the Tx FIFO
        	sendNextFragment();
    }
    if (_lastInterruptFlags[0] & RH_RF22_IRXFFAFULL){
    	// Caution, any delay here may cause a FF overflow
    	// Read some data from the Rx FIFO
    	readNextFragment();
    }
    if (_lastInterruptFlags[0] & RH_RF22_IPKSENT){
    //	Serial.println("IPKSENT");
    	// Transmission does not automatically clear the tx buffer.
    	// Could retransmit if we wanted
    	// RH_RF22 transitions automatically to Idle
    	currentMode = Rfm22ModeIdle;
    }
    if (_lastInterruptFlags[0] & RH_RF22_IPKVALID){
    	uint8_t len = spi.readReg(RH_RF22_REG_4B_RECEIVED_PACKET_LENGTH);
    //	Serial.println("IPKVALID");

    	// May have already read one or more fragments
    	// Get any remaining unread octets, based on the expected length
    	// First make sure we dont overflow the buffer in the case of a stupid length
    	// or partial bad receives
    	if (   len >  RH_RF22_MAX_MESSAGE_LEN
    	    || len < _bufLen)
    	{
    	    currentMode = Rfm22ModeIdle;
    	    clearRxBuf();
    	    return; // Hmmm receiver buffer overflow.
    	}

    	spi.readRegBytes(RH_RF22_REG_7F_FIFO_ACCESS, _buf, len);
    	_rxHeaderTo = spi.readReg(RH_RF22_REG_47_RECEIVED_HEADER3);
    	_rxHeaderFrom = spi.readReg(RH_RF22_REG_48_RECEIVED_HEADER2);
    	_rxHeaderId = spi.readReg(RH_RF22_REG_49_RECEIVED_HEADER1);
    	_rxHeaderFlags = spi.readReg(RH_RF22_REG_4A_RECEIVED_HEADER0);
    	_bufLen = len;
    	_rxBufValid = true;
    	currentMode = Rfm22ModeIdle;
    	_rxBufValid = true;
    	setModeRx();
    }
    if (_lastInterruptFlags[0] & RH_RF22_ICRCERROR){
    //	Serial.println("ICRCERR");
    	clearRxBuf();
    	resetRxFifo();
    	currentMode = Rfm22ModeIdle;
    	setModeRx(); // Keep trying
    }
    if (_lastInterruptFlags[1] & RH_RF22_IPREAVAL){
    //	Serial.println("IPREAVAL");
    	//_lastRssi = (int8_t)(-120 + ((spiRead(RH_RF22_REG_26_RSSI) / 2)));
    	//_lastPreambleTime = millis();
    	resetRxFifo();
    	clearRxBuf();
    }
    return;
}

uint8_t* Receiver::getValues(){
	return _buf;
}

void Receiver::init(){
	_buf[0] = 128;
	_buf[1] = 128;
	Rfm22::init();
	Rfm22::init(); // TODO nejaky bug treba volat 2x aby spi fungovala spravne
	timer.init();
	timer.start();
}

Receiver::~Receiver() {
	// TODO Auto-generated destructor stub
}

