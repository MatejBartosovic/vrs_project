/*
 * Usart.cpp
 *
 *  Created on: Dec 8, 2016
 *      Author: Matejko
 */

#include <Usart2.h>

Usart2::Usart2() {
	// TODO Auto-generated constructor stub

}

Usart2::~Usart2() {
	// TODO Auto-generated destructor stub
}

void Usart2::init(){

	//enable output pins pherifery
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);

	//configure pins
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_USART2);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_USART2);
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 |  GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	//enable uusart pherifery
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);

	//configure usart
	USART_InitTypeDef USART_InitStructure;
	USART_InitStructure.USART_BaudRate = 9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART2, &USART_InitStructure);


	//enable usart interrupts
	USART_ClearFlag(USART2,USART_FLAG_TC);
	USART_ITConfig(USART2, USART_IT_TC, ENABLE);
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);

	//configure nvic
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 8;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	usart = USART2;
	//turn on usart
	USART_Cmd(USART2,ENABLE);

}


