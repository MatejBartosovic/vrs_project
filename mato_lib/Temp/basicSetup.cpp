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

void initLED(){
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
	  GPIO_InitTypeDef initStruct;
	  initStruct.GPIO_Mode = GPIO_Mode_OUT;
	  initStruct.GPIO_OType = GPIO_OType_PP;
	  initStruct.GPIO_PuPd = GPIO_PuPd_UP;
	  initStruct.GPIO_Speed = GPIO_Speed_40MHz;
	  initStruct.GPIO_Pin = GPIO_Pin_5;
	  GPIO_Init(GPIOA,&initStruct);
	return;
}
