/*
 * pwm.c
 *
 *  Created on: 3. 12. 2016
 *      Author: Juraj B
 */

#include <pwm.h>

uint8_t value;

void InitializeTimer()
{
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);

    TIM_TimeBaseInitTypeDef timerInitStructure;
    timerInitStructure.TIM_Prescaler = 160-1; //clock=16MHz
    timerInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
    timerInitStructure.TIM_Period = 2000-1;
    timerInitStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseInit(TIM4, &timerInitStructure);
    TIM_Cmd(TIM4, ENABLE);
}

void InitializePWMChannel()
{
    TIM_OCInitTypeDef outputChannelInit ;
    outputChannelInit.TIM_OCMode = TIM_OCMode_PWM1;
    outputChannelInit.TIM_Pulse = 150;
    outputChannelInit.TIM_OutputState = TIM_OutputState_Enable;
    outputChannelInit.TIM_OCPolarity = TIM_OCPolarity_High;

    TIM_OC1Init(TIM4, &outputChannelInit);
    TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_TIM4);

}

void InitializePWMChannel2()
{
    TIM_OCInitTypeDef outputChannelInit ;
    outputChannelInit.TIM_OCMode = TIM_OCMode_PWM1;
    outputChannelInit.TIM_Pulse = 150;
    outputChannelInit.TIM_OutputState = TIM_OutputState_Enable;
    outputChannelInit.TIM_OCPolarity = TIM_OCPolarity_High;

    TIM_OC3Init(TIM4, &outputChannelInit);
    TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Enable);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource8, GPIO_AF_TIM4);

}

void vystupinitGPIO()
{
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);

    GPIO_InitTypeDef gpioStructure;
    gpioStructure.GPIO_Pin = GPIO_Pin_6 ;
    gpioStructure.GPIO_Mode = GPIO_Mode_AF;
    gpioStructure.GPIO_OType = GPIO_OType_PP;
    gpioStructure.GPIO_PuPd  = GPIO_PuPd_UP;
    gpioStructure.GPIO_Speed = GPIO_Speed_40MHz;
    GPIO_Init(GPIOB, &gpioStructure);

}

void vystupinit2GPIO()
{
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);

    GPIO_InitTypeDef gpioStructure;
    gpioStructure.GPIO_Pin = GPIO_Pin_8 ;
    gpioStructure.GPIO_Mode = GPIO_Mode_AF;
    gpioStructure.GPIO_OType = GPIO_OType_PP;
    gpioStructure.GPIO_PuPd  = GPIO_PuPd_UP;
    gpioStructure.GPIO_Speed = GPIO_Speed_40MHz;
    GPIO_Init(GPIOB, &gpioStructure);

}



void sysClockSetup()
{
	RCC_HSICmd(ENABLE);
	while(RCC_GetFlagStatus(RCC_FLAG_HSIRDY) == RESET);

	RCC_SYSCLKConfig(RCC_CFGR_SW_HSI);
	SystemCoreClockUpdate();
}


void usart_init()
{
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);

		GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_USART2);
		GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_USART2);

		GPIO_InitTypeDef GPIO_usrt;

		GPIO_usrt.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3;
		GPIO_usrt.GPIO_Mode = GPIO_Mode_AF;
		GPIO_usrt.GPIO_OType = GPIO_OType_PP;
		GPIO_usrt.GPIO_PuPd = GPIO_PuPd_NOPULL;
		GPIO_usrt.GPIO_Speed = GPIO_Speed_40MHz;

		GPIO_Init(GPIOA,&GPIO_usrt);

		RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);

		USART_InitTypeDef USART_InitStructure;
		USART_InitStructure.USART_BaudRate = 9600;
		USART_InitStructure.USART_WordLength = USART_WordLength_8b;
		USART_InitStructure.USART_StopBits = USART_StopBits_1;
		USART_InitStructure.USART_Parity = USART_Parity_No;
		USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
		USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
		USART_Init(USART2, &USART_InitStructure);

		  /* Enable the USARTx Interrupt */
		NVIC_InitTypeDef NVIC_InitStructure;
		NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitStructure);

		//interrupt
		//USART_ITConfig(USART2, USART_IT_TC, ENABLE);
		USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);

		USART_Cmd(USART2, ENABLE);

}

extern "C" void USART2_IRQHandler(void){
	// this is the interrupt request handler (IRQ) for ALL USART1 interrupts

	if (USART_GetITStatus(USART2, USART_IT_RXNE)==SET) {

		USART_ClearITPendingBit(USART2, USART_IT_RXNE);
		value = USART_ReceiveData(USART2);
		// Send data back for verification
		USART_SendData(USART2,value);

		TIM4->CCR1 = value; // pre servo, PWM = 2000, value = cca 120-180

	}

	return;
}



