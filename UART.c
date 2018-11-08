/*
 * UART.c
 *
 * Created: 7/25/2018 5:23:54 PM
 *  Author: VladBaciu
 */ 

#include <avr/io.h>
#include <avr/sfr_defs.h>
#include <stdio.h>

void uart_init(unsigned int baudRate,unsigned long fcpu)
{
	unsigned int ubrr= fcpu/16/baudRate-1;
	
	UBRR0H = (unsigned char) (ubrr>>8);  // first 8 bits
	UBRR0L = (unsigned char) ubrr;
	UCSR0B = (1<<RXEN0)|(1<<TXEN0);  // rx tx module 0
	UCSR0C = (1<<USBS0)|(3<<UCSZ00);  // 8 bit , 2 stop (frame format)
	
	UCSR0B |= (1<<RXCIE0); // enable interrupts
	
}

void usart_send_byte(char byte, FILE *stream) {
	if (byte == '\n') {
		usart_send_byte('\r',stream);
	}
	loop_until_bit_is_set(UCSR0A,UDRE0);
	UDR0 = byte;
}

