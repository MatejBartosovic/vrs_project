/*
 * pwm.h
 *
 *  Created on: 3. 12. 2016
 *      Author: Juraj B
 */

#ifndef PWM_H_
#define PWM_H_

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stddef.h>
#include "stm32l1xx.h"

#define SCALE_CONST .39215686



void InitializeTimer();
void InitializePWMChannel();
void InitializePWMChannel2();
void vystupinitGPIO();
void vystupinit2GPIO();
void usart_init();
void setPwmValues(uint8_t* val);



#endif /* PWM_H_ */
