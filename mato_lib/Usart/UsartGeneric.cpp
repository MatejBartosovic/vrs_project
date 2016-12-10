/*
 * UsartGeneric.cpp
 *
 *  Created on: Dec 8, 2016
 *      Author: Matejko
 */

#include <UsartGeneric.h>

UsartGeneric::UsartGeneric() : usart_write_buffer_it(0), usart_send_it(0), usart_read_buffer_it(0), usart_read_it(0){
	// TODO Auto-generated constructor stub

}

uint8_t UsartGeneric::read(uint8_t* data,uint8_t len){
	for(int i=0;i<len;i++){
		if(usart_read_it==usart_read_buffer_it)
			return i;
		*(data+i) = usart_read_buffer[usart_read_it];
		usart_read_it++;
	}
	return len;

}
void UsartGeneric::write(uint8_t* data,uint8_t len){
	for(uint8_t  i= 0;i<len;i++){
			usart_write_buffer[usart_write_buffer_it] = *(data+i);
			usart_write_buffer_it ++;
		}
		initWrite(len);

}
uint8_t UsartGeneric::availableBytes(){
	return (usart_read_buffer_it - usart_read_it);
}
void UsartGeneric::flush(){
	usart_send_it = usart_write_buffer_it;
}

uint8_t UsartGeneric::writableBytes(){
		return (usart_write_buffer_it - usart_send_it);
}

void UsartGeneric::irqHandler (void) {

	if(USART_GetFlagStatus(usart, USART_FLAG_RXNE) == SET){
		usart_read_buffer[usart_read_buffer_it] = USART_ReceiveData(usart);
		USART_ClearFlag(usart, USART_FLAG_RXNE);
		usart_read_buffer_it++;
		return;
	}
	if(USART_GetFlagStatus(usart,USART_FLAG_TC) == SET){
		USART_ClearFlag(usart,USART_FLAG_TC);
		if(usart_send_it != usart_write_buffer_it){
			USART_SendData(usart,usart_write_buffer[usart_send_it]);
			usart_send_it++;
		}
		return;
	}
}

UsartGeneric::~UsartGeneric() {
	// TODO Auto-generated destructor stub
}

/*
 *
 * Private function
 *
 * */


void UsartGeneric::initWrite(uint8_t new_bytes){
	if(!(writableBytes() - new_bytes )){
		USART_SendData(usart,usart_write_buffer[usart_send_it]);
		usart_send_it++;
	}
}

