#include<avr/io.h>
#define F_CPU 16000000UL
#include<util/delay.h>

int led_all_onoff(int x,int d)
{
	DDRD = 0xFF; 
	
	for(int i=0 ; i<x ; i++)
	{
		PORTD = 0x00;
		_delay_ms(d);
		PORTD = 0xFF; 
		_delay_ms(d);
	}
}

int main()
{
	led_all_onoff(5,500);
}
