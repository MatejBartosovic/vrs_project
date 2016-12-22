/*
 * lib_p.h
 *
 *  Created on: 20. 12. 2016
 *      Author: Patrik Bakyta
 */

#ifndef LIB_P_H_
#define LIB_P_H_


void InitSYSTEMCLOCK(void);
void EnableInterrupt(uint8_t IRQChannel, uint8_t Preemption, uint8_t Sub);

void InitTIM(TIM_TypeDef* Timer, uint16_t Prescaler, uint32_t Period);
extern "C" void TIM2_IRQHandler(void);

void InitUART2viaUSB(void);
extern "C" void USART2_IRQHandler(void);

void initADC1withDMA1(void);
void ADC_print(uint8_t array_vals[2]);
int *remap(uint8_t array_values[2]);
char *INTconversionCHAR(uint8_t value);


#endif /* LIB_P_H_ */
