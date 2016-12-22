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

template <typename T = uint8_t>
class JoystickGeneric {

	typedef struct{
		T x;
		T y;
	}joystickValuse;

public:
	JoystickGeneric();
	void virtual init() = 0;
	joystickValuse virtual readValue() = 0;
	virtual ~JoystickGeneric();
protected:
	//structure which store current joystick data
	joystickValuse values;
};

#endif /* JOYSTICKGENERIC_H_ */
