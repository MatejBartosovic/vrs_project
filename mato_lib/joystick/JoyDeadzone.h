/*
 * JoyReader.h
 *
 *  Created on: Dec 23, 2016
 *      Author: Matejko
 */

#ifndef JOYDEADZONE_H_
#define JOYDEADZONE_H_

/*
 * Deadzone class
 * */

#include <JoystickGeneric.h>
#include <string.h>

template <class T>
class JoyDeadzone {
public:
	//constructor
	//JoystickGeneric - pointer to class which inherit from JoystickGeneric class
	JoyDeadzone(
			JoystickGeneric<uint8_t> &joy,
			T deadzone = 10, T joyMid = 128) :
			joy(joy),
			joyMid(joyMid),
			deadzoneP(joyMid + deadzone),
			deadzoneN(joyMid - deadzone){
	}

	//set new joystick
	void setJoy(JoystickGeneric<uint8_t> &joy){
		this->joy = joy;
	}

	//set new deadzone
	void setDeadzone(T deadzone){
		this->deadzoneP = joyMid + deadzone;
		this->deadzoneN = joyMid - deadzone;
	}

	//set new joystick middle value
	void setJoyMid(T joyMid){
		this->joyMid = joyMid;
	}

	//init
	void init(){
		joy.init();
		joy.start();
	}

	//return joystick values
	T *getValues(){
		//read values to local variable
		memcpy(readerValues,joy.readValue(),2);
		if((readerValues[0] < deadzoneP) &&(readerValues[0] > deadzoneN))
			readerValues[0] = joyMid;
		if((readerValues[1] < deadzoneP) &&(readerValues[1] > deadzoneN))
			readerValues[1] = joyMid;
		return readerValues;
	}
	virtual ~JoyDeadzone(){
	}
private:

protected:
	JoystickGeneric<T> &joy;
	T joyMid;
	T deadzoneP;
	T deadzoneN;
	T readerValues[2];
};

#endif /* JOYDEADZONE_H_ */
