/*
 * Spi1.cpp
 *
 *  Created on: Dec 10, 2016
 *      Author: Matejko
 */

#include <Spi1.h>

Spi1::Spi1() {
	// TODO Auto-generated constructor stub

}

void Spi1::init(){
	GPIO_InitTypeDef GPIO_InitStructure;

		//inti Spi pins
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);

		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;

		//PB3 - SPI2_SCK
		//PB4 - SPI2_MISO
		//PB5 - SPI2_MOSI
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5;
		GPIO_Init(GPIOB, &GPIO_InitStructure);


		// init cs
		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);

		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
		GPIO_Init(GPIOB, &GPIO_InitStructure);
		GPIO_SetBits(GPIOB,GPIO_Pin_10);

		//init Spi
		GPIO_PinAFConfig(GPIOB, GPIO_PinSource3, GPIO_AF_SPI1);
		GPIO_PinAFConfig(GPIOB, GPIO_PinSource4, GPIO_AF_SPI1);
		GPIO_PinAFConfig(GPIOB, GPIO_PinSource5, GPIO_AF_SPI1);

		SPI_InitTypeDef  SPI_InitStructure;
		SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;
		SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
		SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
		SPI_InitStructure.SPI_CRCPolynomial = SPI_CRC_Rx;
		SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
		SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
		SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
		SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
		SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;

		/* Apply SPI configuration after enabling it */
		SPI_Init(SPI1, &SPI_InitStructure);


		/*SPI_I2S_ClearFlag(SPI1,SPI_I2S_FLAG_TXE);
		SPI_I2S_ITConfig(SPI1,SPI_I2S_IT_RXNE,ENABLE);
		SPI_I2S_ITConfig(SPI1,SPI_I2S_IT_TXE,ENABLE); // tento interrupt robi pekne sracky !!Nezapinat

		NVIC_InitTypeDef NVIC_InitStructure;
		NVIC_InitStructure.NVIC_IRQChannel = SPI1_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 7;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitStructure);*/

		/* SPI Peripheral Enable */
		spi = SPI1;
		cs = GPIO_Pin_10;
		gpio_cs = GPIOB;
		SPI_Cmd(SPI1, ENABLE);
}

Spi1::~Spi1() {
	// TODO Auto-generated destructor stub
}

