/*
 * Timer.h
 *
 *  Created on: Dec 22, 2016
 *      Author: Matejko
 */

#ifndef TIMER_H_
#define TIMER_H_

#include <stddef.h>
#include "stm32l1xx.h"


class Timer {
public:
	Timer(	TIM_TypeDef* Timer,
			uint16_t Prescaler,
			uint32_t Period,
			uint16_t TIM_CounterMode = TIM_CounterMode_Up,
			uint16_t TIM_ClockDivision = 0,
			uint8_t NVIC_IRQChannelPreemptionPriority = 8,
			uint8_t NVIC_IRQChannelSubPriority = 0);
	bool init();
	void start();
	void stop();
	virtual ~Timer();
private:
	TIM_TimeBaseInitTypeDef timerInitStructure;
	NVIC_InitTypeDef nvicStructure;
	TIM_TypeDef* timer;
};

#endif /* TIMER_H_ */
