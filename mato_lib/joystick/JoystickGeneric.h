/*
 * JoystickGeneric.h
 *
 *  Created on: Dec 22, 2016
 *      Author: Matejko
 */

#ifndef JOYSTICKGENERIC_H_
#define JOYSTICKGENERIC_H_

#include <stddef.h>
#include "stm32l1xx.h"

template <typename type>
struct joystickValuse{
	type x;
	type y;
};

template <typename T>
class JoystickGeneric {


public:
	JoystickGeneric(){};
	void virtual init() = 0;
	T* readValue(){
		return values;
	};
	virtual void start() = 0;
	virtual void stop() = 0;
	virtual ~JoystickGeneric(){};
protected:
	//structure which store current joystick data
	T values[2];
};

#endif /* JOYSTICKGENERIC_H_ */
