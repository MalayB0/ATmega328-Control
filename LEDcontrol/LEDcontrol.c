#include <avr/io.h>
#define F_CPU 16000000UL
#include<util/delay.h>

int LED_ALL_ON_OFF()
{
	for(int i=0;i<3;i++)
	{
		PORTD = 0x0F;
		PORTB = 0xF0;
		_delay_ms(100);
		PORTD = 0xF0;
		PORTB = 0x0F;
		_delay_ms(100);
	}
}

int LED_ALTERNATING()
{
	for(int i=0;i<3;i++)
	{
		PORTD = 0xA0;
		PORTB = 0x0A;
		_delay_ms(100);
		PORTD = 0x50;
		PORTB = 0x05;
		_delay_ms(100);
	}

	for(int i=0;i<3;i++)
	{
		PORTD = 0x0F;
		PORTB = 0xFF;
		_delay_ms(100);
		PORTD = 0xFF;
		PORTB = 0xF0;
		_delay_ms(100);
	}

	for(int i=0;i<3;i++)
	{
		PORTD = 0xC0;
		PORTB = 0x03;
		_delay_ms(100);
		PORTD = 0x30;
		PORTB = 0x0C;
		_delay_ms(100);
	}
}

int LED_SHIFT()
{
	for(int j = 0;j<5;j++)
	{
		PORTD = 0xFF;
		PORTB = 0xF7;
		for(int i=0;i<5;i++)
		{
			_delay_ms(100);
			PORTB = ~(0x08>>i);
		}
		for(int i=0;i<4;i++)
		{
			PORTD = ~(0x80>>i);
			_delay_ms(100);
		}
		for(int i=1;i<5;i++)
		{
			_delay_ms(100);
			PORTD = ~(0x10<<i);
		}
		for(int i=0;i<4;i++)
		{
			PORTB = ~(0x01<<i);
			_delay_ms(100);
		}
	}
}


int main()
{
	while(1)
	{
		DDRD = 0xFF;
		DDRB = 0xFF;
		LED_ALL_ON_OFF();
		LED_ALTERNATING();
		LED_SHIFT();
	}
}
