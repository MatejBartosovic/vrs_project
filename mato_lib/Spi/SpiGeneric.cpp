/*
 * SpiGeneric.cpp
 *
 *  Created on: Dec 10, 2016
 *      Author: Matejko
 */

#include <SpiGeneric.h>

SpiGeneric::SpiGeneric() {
	// TODO Auto-generated constructor stub

}

void SpiGeneric::writeRegBytes(uint8_t reg,uint8_t* buf,uint8_t len){

	GPIO_ResetBits(gpio_cs,cs);
	spi->DR = (reg | SPI_WRITE_FLAG);
	waitTxe();
	for(int i=0;i<len;i++){
		spi->DR = *(buf+i);
		waitTxe();
	}
	GPIO_SetBits(gpio_cs,cs);
}

void SpiGeneric::writeReg(uint8_t reg,uint8_t val){
	GPIO_ResetBits(gpio_cs,cs);
	spi->DR = (reg | SPI_WRITE_FLAG);
	waitTxRx();
	spi->DR = val;
	waitTxe();
	GPIO_SetBits(gpio_cs,cs);
}


uint8_t SpiGeneric::readReg(uint8_t reg){
	GPIO_ResetBits(gpio_cs,cs);
	spi->DR = (reg &~ SPI_WRITE_FLAG);
	waitTxRx();
	spi->DR;
	spi->DR = 0x00;
	waitTxRx();
	GPIO_SetBits(gpio_cs,cs);
	return  spi->DR;


}

void SpiGeneric::readRegBytes(uint8_t reg,uint8_t* buf,uint8_t len){
	GPIO_ResetBits(gpio_cs,cs);
	spi->DR = (reg &~ SPI_WRITE_FLAG);
	waitTxRx();
	spi->DR;
	for(int i =0;i<len;i++){
		spi->DR = 0x00;
		waitTxRx();
		*(buf+i) = spi->DR;
	}
	GPIO_SetBits(gpio_cs,cs);
}

void SpiGeneric::writeRead(uint8_t* buf,uint8_t len){
	GPIO_ResetBits(gpio_cs,cs);
	for(int i =0;i<len;i++){
		spi->DR = *(buf+i);
		waitTxRx();
		*(buf+i) = spi->DR;
	}
	GPIO_SetBits(gpio_cs,cs);
}

void SpiGeneric::waitRxne(){
	while(!(spi->SR & SPI_I2S_FLAG_RXNE));
}

void SpiGeneric::waitTxe(){
	while(!(spi->SR & SPI_I2S_FLAG_RXNE));
}
void SpiGeneric::waitTxRx(){
	while(!(spi->SR & SPI_I2S_FLAG_RXNE));
	while(!(spi->SR & SPI_I2S_FLAG_RXNE));
}

SpiGeneric::~SpiGeneric() {
	// TODO Auto-generated destructor stub
}

/*
 *
 * Private functions
 *
 * */


