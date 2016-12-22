/*
 * Timer.cpp
 *
 *  Created on: Dec 22, 2016
 *      Author: Matejko
 */

#include <Timer.h>

Timer::Timer(TIM_TypeDef* timer,
		uint16_t Prescaler,
		uint32_t Period,
		uint16_t TIM_CounterMode,
		uint16_t TIM_ClockDivision,
		uint8_t NVIC_IRQChannelPreemptionPriority,
		uint8_t NVIC_IRQChannelSubPriority) {
	timerInitStructure.TIM_Prescaler = Prescaler-1;
	timerInitStructure.TIM_CounterMode = TIM_CounterMode;
	timerInitStructure.TIM_Period = Period-1;
	timerInitStructure.TIM_ClockDivision = TIM_ClockDivision;

    nvicStructure.NVIC_IRQChannelPreemptionPriority = NVIC_IRQChannelPreemptionPriority;
    nvicStructure.NVIC_IRQChannelSubPriority = NVIC_IRQChannelSubPriority;

	this->timer = timer;
}

bool Timer::init(){
	uint8_t IRQChannel;
	if (timer==TIM2) {
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
		IRQChannel = TIM2_IRQn;
	} else if (timer==TIM3) {
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
		IRQChannel = TIM3_IRQn;
	} else if (timer==TIM4) {
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
		IRQChannel = TIM4_IRQn;
	} else if (timer==TIM5) {
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);
		IRQChannel = TIM5_IRQn;
	} else if (timer==TIM6) {
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);
		IRQChannel = TIM6_IRQn;
	} else if (timer==TIM7) {
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7, ENABLE);
		IRQChannel = TIM7_IRQn;
	}
	else
		return false;

	TIM_TimeBaseInit(timer, &timerInitStructure);

	TIM_ITConfig(timer, TIM_IT_Update, ENABLE);
	TIM_ClearITPendingBit(timer, TIM_IT_Update);

    nvicStructure.NVIC_IRQChannel = IRQChannel;
    nvicStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&nvicStructure);
	// Enable TIM update Interrupt source

	return true;
	}

void Timer::start(){
	TIM_Cmd(timer, ENABLE);
}

void Timer::stop(){
	TIM_Cmd(timer, DISABLE);
}

Timer::~Timer() {
	stop();
}

