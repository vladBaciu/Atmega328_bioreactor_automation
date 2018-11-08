/*
 * UART.h
 *
 * Created: 7/25/2018 5:24:10 PM
 *  Author: VladBaciu
 */ 


#ifndef UART_H_
#define UART_H_
#include <stdio.h>

void uart_init(unsigned int baud,unsigned long fcpu);

void usart_send_byte(char byte, FILE *stream);



FILE uart_output = FDEV_SETUP_STREAM(usart_send_byte, NULL, _FDEV_SETUP_WRITE);


#endif /* UART_H_ */