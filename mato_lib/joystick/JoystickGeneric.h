/*
 * JoystickGeneric.h
 *
 *  Created on: Dec 22, 2016
 *      Author: Matejko
 */

#ifndef JOYSTICKGENERIC_H_
#define JOYSTICKGENERIC_H_

/*
 * Template class to reading joystick data
 * */

#include <stddef.h>
#include "stm32l1xx.h"

/*
 * Template class to joystick
 * */

template <typename T>
class JoystickGeneric {


public:
	//constructor
	JoystickGeneric(){};

	//init - must by implemented by by inherited class
	void virtual init() = 0;

	//return current values
	T* readValue(){
		return values;
	};

	//start reading values from AD converter
	virtual void start() = 0;

	//stop  reading values from AD converter
	virtual void stop() = 0;

	//destructor
	virtual ~JoystickGeneric(){};
protected:
	//array which store current joystick data
	T values[2];
};

#endif /* JOYSTICKGENERIC_H_ */
