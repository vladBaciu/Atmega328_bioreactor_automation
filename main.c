/*
 * UnicaitProject.c
 *
 * Created: 7/25/2018 4:54:06 PM
 * Author : VladBaciu
 */ 
#define F_CPU 16000000L 
#define BAUD 9600
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdint.h>
#include "UART.h"
#include "Relay.h"
#include "ADC.h"
#include "bmp180.h"

#define THRESHOLD	 550
#define ADC_PIN	 0

volatile unsigned char  data;
static unsigned int  instruction,minute_timer;
static int pressBar;
volatile unsigned int flag=0;
static int adc_treshold;
static long aux;
static long temp;

void init_timer1();
void init_timer0();


int main(void)
{
	relay_init();
	init_timer1();
	init_timer0();
	
	uart_init(BAUD,F_CPU);
	adc_init();
	
	sei();
    stdout = &uart_output;
	
	
  //calibrate_bmp180();
  for (;;) {
	
	printf("as");
	if(flag==1)
			{
				
				instruction=data;
				//printf("%x",instruction);
				relay_control(instruction);
				flag=0;
			}
	
  }
  return 0;
}
void init_timer1()
{
	
	// ****************start INIT TIMER 1-transmit sensor data at 1m**************   //
	 minute_timer=0;
	 OCR1A = 0x3D08;
	 TCCR1B |= (1 << WGM12);
	 TIMSK1 |= (1 << OCIE1A);
	 TCCR1B |= (1 << CS12) | (1 << CS10);  //15.6 khz
	// ****************stop INIT TIMER 1**************   //


}
ISR (TIMER1_COMPA_vect)
{

	minute_timer++;
	if(minute_timer==60)
	{
		aux= getPressure();
		temp=aux/100;
		temp=temp%1000;
		
		printf("t%d.%02d", getTemp()/10,	(getTemp()%10) *10);
		printf("p%ld.%03ld",aux/100000,temp);  //Pa to bar
		
		if(adc_treshold)
		{
			printf("level1");
			adc_treshold=0;		
		}
		else printf("level0");
	
		minute_timer=0;
	}
	
}

void init_timer0()
{
	TCCR0A = (1 << WGM01);
	OCR0A = 195.3;
	TIMSK0 = (1 << OCIE0A);
	TCCR0B = (1 << CS02) | (1 << CS00);
}


ISR(TIMER0_COMPA_vect)
{
	
	if ((unsigned int)adc_read(ADC_PIN) > THRESHOLD)
			adc_treshold=1;
	
}


ISR(USART_RX_vect) {
	data = UDR0;
	flag=1;
}
