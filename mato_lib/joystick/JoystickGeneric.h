/*
 * JoystickGeneric.h
 *
 *  Created on: Dec 22, 2016
 *      Author: Matejko
 */

#ifndef JOYSTICKGENERIC_H_
#define JOYSTICKGENERIC_H_

class JoystickGeneric {
public:
	JoystickGeneric();
	void virtual init() = 0;
	void virtual readValue();
	virtual ~JoystickGeneric();
protected:
};

#endif /* JOYSTICKGENERIC_H_ */
