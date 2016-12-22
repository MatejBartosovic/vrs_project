/*
 * Joystick.h
 *
 *  Created on: Dec 22, 2016
 *      Author: Matejko
 */

#ifndef JOYSTICK_H_
#define JOYSTICK_H_

#include <JoystickGeneric.h>

class Joystick : JoystickGeneric{
public:
	Joystick();
	void init();
	virtual ~Joystick();
};

#endif /* JOYSTICK_H_ */
