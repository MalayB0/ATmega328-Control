#include<avr/io.h>
#define F_CPU 16000000UL
#include<util/delay.h>

int ledshift()
{
	DDRD = 0xFF;
	
	for(int i = 0; i < 8; i++)
	{
		PORTD = 0x01<<i;
		_delay_ms(500);
	}
	for(int j = 0; j < 9; j++)
	{
		PORTD = 0x40>>j;
		_delay_ms(500);
	}

}

int segD[10] = {0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x27,0x7F,0x6F};
int segB[10] = {0x3F,0x06,0x1B,0x0F,0x26,0x2D,0x3D,0x07,0x3F,0x2F};
int segC[10] = {0x00,0x00,0x01,0x01,0x01,0x01,0x01,0x00,0x01,0x01};

int segment()
{
	
	DDRD = 0xFF;
	DDRB = 0xFF;
	DDRC = 0xFF;
	for(int i = 0; i<10;i++)
	{
		PORTD = segD[i];
		PORTB = segB[0];
		PORTC = segC[0];
		for(int j = 0; j<10; j++)
		{
			PORTB = segB[j];
			PORTC = segC[j];
			_delay_ms(200);
		}
	}
	PORTD = segD[0];
	PORTB = segB[0];
	PORTC = segC[0];
}
int main()
{
	DDRD = 0xFF;
	DDRB = 0xFF;
	DDRC = 0xFF;
	segment();

}
