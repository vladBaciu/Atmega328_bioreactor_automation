/*
 * Relee.c
 *
 * Created: 8/1/2018 11:39:55 AM
 *  Author: VladBaciu
 */
#include <avr/io.h> 
#include "Relay.h"
#include <util/delay.h>
void relay_init()
{
	DDRB= (1<<PB0) | (1<<PB1) | (1<<PB2) | (1<<PB3);   //set relay pins as output
	PORTB=(1<<PB3);
	PORTB |= 0x00; // initial state 0 logic
}
void relay_control(unsigned char comand)
{
	if (comand == 0x31)  // "1"
	{
				
					PORTB ^= (1<<AIR_PUMP_2);
					_delay_ms(1000);
					PORTB ^= (1<<AIR_PUMP_2);
							
							PORTB |= (1<<FED_PUMP) | (1<<AIR_PUMP);
							 
						
							
							      //start fed and air pumps
	}
	else if(comand == 0x32 )  // "2"
	{
							
							PORTB ^= (1<<FED_PUMP);							//stop fed pump
							
	}
	else if(comand == 0x33) // "3"
	{
		
						  PORTB |= (1<<DISCHARGE_PUMP);					//start discharge_pump
		
	}
	else if(comand == 0x34) // "4"
	{
		
		
		PORTB ^= (1<<AIR_PUMP_2);
		_delay_ms(1000);
		PORTB ^= (1<<AIR_PUMP_2);
			
						PORTB ^= (1<<DISCHARGE_PUMP) | (1<<AIR_PUMP);	//stop discharge_pump and air pump
						
	}
	else if(comand ==0x35)
	{
		
		
		
		
						PORTB ^= (1<<DISCHARGE_PUMP);
						
	}	
	else if (comand == 0x36)
	{	
		
		PORTB ^= (1<<AIR_PUMP_2);
		_delay_ms(1000);
		PORTB ^= (1<<AIR_PUMP_2);
						PORTB &=0x00;
						
						
		
	}
	else if (comand == 0x30)
	{
		PORTB ^= (1<<AIR_PUMP_2);
		_delay_ms(1000);
		PORTB ^= (1<<AIR_PUMP_2);		
							PORTB ^= PORTB;	
																//stop pumps when the user close the application
	
	} 
	
			
									
								

				
}