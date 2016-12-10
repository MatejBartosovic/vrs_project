/*
 * basicSetup.cpp
 *
 *  Created on: Dec 10, 2016
 *      Author: Matejko
 */
#include <basicSetup.h>

void sysClockSetup(){
	RCC_HSICmd(ENABLE);
	while(RCC_GetFlagStatus(RCC_FLAG_HSIRDY) == RESET);

	RCC_SYSCLKConfig(RCC_CFGR_SW_HSI);
	SystemCoreClockUpdate();
}

