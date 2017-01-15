/*
 * Timer.h
 *
 *  Created on: Dec 22, 2016
 *      Author: Matejko
 */

#ifndef TIMER_H_
#define TIMER_H_
/*
 * Class to timer interface.
 * This class configure timer to generate periodic interrupts
 * Interrupt function had to by implemented
 * */
#include <stddef.h>
#include "stm32l1xx.h"

class Timer {
public:
	//constructor
	Timer(	TIM_TypeDef* Timer,
			uint16_t Prescaler,
			uint32_t Period,
			uint16_t TIM_CounterMode = TIM_CounterMode_Up,
			uint16_t TIM_ClockDivision = 0,
			uint8_t NVIC_IRQChannelPreemptionPriority = 8,
			uint8_t NVIC_IRQChannelSubPriority = 0);

	//inicialize timer
	bool init();

	//start timer
	void start();

	//stop timer
	void stop();

	//destructor
	virtual ~Timer();
private:
	TIM_TimeBaseInitTypeDef timerInitStructure;
	NVIC_InitTypeDef nvicStructure;
	TIM_TypeDef* timer;
};

#endif /* TIMER_H_ */
