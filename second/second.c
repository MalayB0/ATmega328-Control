#include<avr/io.h>
#define F_CPU 16000000UL
#include<util/delay.h>

int ledmain()
{
	DDRD = 0xFF; 
	
	for(int i=0 ; i<3 ; i++)
	{
		int val = 0xAA;
		PORTD = val;
		_delay_ms(500);
		PORTD = ~val; 
		_delay_ms(500);
	}

	
	for(int i=0 ; i<4 ; i++)
	{
		PORTD = 0xF0;
		_delay_ms(500);
		PORTD = 0x0F; 
		_delay_ms(500);
	}

	for(int i=0 ; i<5 ; i++)
	{
		PORTD = 0xC3;
		_delay_ms(500);
		PORTD = 0x3C; 
		_delay_ms(500);
	}
	PORTD = 0xFF;
}

int ledshift()
{
	DDRD = 0xFF; 

		PORTD = 0xFE;
		_delay_ms(500);
		PORTD = 0xFD;
		_delay_ms(500);
		PORTD = 0xFB;
		_delay_ms(500);
		PORTD = 0xF7;
		_delay_ms(500);
		PORTD = 0xEF;
		_delay_ms(500);
		PORTD = 0xDF;
		_delay_ms(500);
		PORTD = 0xBF;
		_delay_ms(500);
		PORTD = 0x7F;
		_delay_ms(500);
		PORTD = 0xBF;
		_delay_ms(500);
		PORTD = 0xDF;
		_delay_ms(500);
		PORTD = 0xEF;
		_delay_ms(500);
		PORTD = 0xF7;
		_delay_ms(500);
		PORTD = 0xFB;
		_delay_ms(500);
		PORTD = 0xFD;
		_delay_ms(500);
		PORTD = 0xFE;
		_delay_ms(500);	
	
}

void main()
{
	ledshift();
}
