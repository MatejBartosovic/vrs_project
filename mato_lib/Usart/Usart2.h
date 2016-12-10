/*
 * Usart.h
 *
 *  Created on: Dec 8, 2016
 *      Author: Matejko
 */

#ifndef USART2_H_
#define USART2_H_

#include <UsartGeneric.h>

class Usart2 : public UsartGeneric  {
public:
	Usart2();
	void init();
	virtual ~Usart2();
private:
};

#endif /* USART2_H_ */
