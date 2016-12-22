/*
 * Joystick.h
 *
 *  Created on: Dec 22, 2016
 *      Author: Matejko
 */

#ifndef JOYSTICK_H_
#define JOYSTICK_H_

#include <JoystickGeneric.h>

template <typename T>
class Joystick : JoystickGeneric<T> {
public:
	Joystick();
	void init(){
		 ADC_InitTypeDef       ADC_InitStruct;
		    ADC_CommonInitTypeDef ADC_CommonInitStruct;
		    DMA_InitTypeDef       DMA_InitStruct;
		    GPIO_InitTypeDef      GPIO_InitStruct;

		    // Enable DMA2 clocks
		    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
		    // Enable GPIOA clocks
		    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
		    // Enable ADC1 clocks
		    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);

		    // DMA1 Stream0 channel0 configuration
		    DMA_InitStruct.DMA_M2M = DMA_M2M_Disable;
		    DMA_InitStruct.DMA_PeripheralBaseAddr = (uint32_t)&ADC1->DR; // ADC1's data register
		    DMA_InitStruct.DMA_MemoryBaseAddr = (uint32_t)&values;
		    DMA_InitStruct.DMA_DIR = DMA_DIR_PeripheralSRC;
		    DMA_InitStruct.DMA_BufferSize = 2;
		    DMA_InitStruct.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
		    DMA_InitStruct.DMA_MemoryInc = DMA_MemoryInc_Enable;
		    DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte; // Reads 8 bit values
		    DMA_InitStruct.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte; // Stores 8 bit values
		    DMA_InitStruct.DMA_Mode = DMA_Mode_Circular;
		    DMA_InitStruct.DMA_Priority = DMA_Priority_High;
		    DMA_Init(DMA1_Channel1, &DMA_InitStruct);
		    DMA_Cmd(DMA1_Channel1, ENABLE);

		    // Configure GPIO pins
		    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1; // PA0 - Channel 0, PA1 - Channel 1
		    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AN; // The pins are configured in analog mode
		    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL; // We don't need any pull up or pull down
		    GPIO_Init(GPIOC, &GPIO_InitStruct); // Initialize GPIOA pins with the configuration

		    // ADC Common Init
		    ADC_CommonInitStruct.ADC_Prescaler = ADC_Prescaler_Div2;
		    ADC_CommonInit(&ADC_CommonInitStruct);

		    // ADC1 Init
		    ADC_DeInit(ADC1);
		    ADC_InitStruct.ADC_Resolution = ADC_Resolution_8b; // Input voltage is converted into a 8bit int (max 255)
		    ADC_InitStruct.ADC_ScanConvMode = ENABLE; // The scan is configured in multiple channels
		    ADC_InitStruct.ADC_ContinuousConvMode = ENABLE; // Continuous conversion: input signal is sampled more than once
		    ADC_InitStruct.ADC_ExternalTrigConv = 0;
		    ADC_InitStruct.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
		    ADC_InitStruct.ADC_DataAlign = ADC_DataAlign_Right; // Data converted will be shifted to right
		    ADC_InitStruct.ADC_NbrOfConversion = 2;
		    ADC_Init(ADC1, &ADC_InitStruct); // Initialize ADC with the configuration

		    // Select the channels to be read from
		    ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_96Cycles); // PA0
		    ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 2, ADC_SampleTime_96Cycles); // PA1

		    // Enable DMA request after last transfer (Single-ADC mode)
		    ADC_DMARequestAfterLastTransferCmd(ADC1, ENABLE);
		    // Enable ADC1 DMA
		    ADC_DMACmd(ADC1, ENABLE);
		    // Enable ADC1
		    ADC_Cmd(ADC1, ENABLE);
		    // Start ADC1 Software Conversion
		    ADC_SoftwareStartConv(ADC1);

		    return;

	}

	joystickValuse readValue() {
		return values;
	}
	virtual ~Joystick();
};

#endif /* JOYSTICK_H_ */
