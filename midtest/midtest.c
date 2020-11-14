#include<avr/io.h>
#define F_CPU 16000000UL
#include<util/delay.h>
#include<avr/interrupt.h>

unsigned char s[] = {0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x27,0x7F,0x6F,0x77,0x7C,0x39,0x5E,0x79,0x71};

void LED()
{
	PORTB = 0xFF;
	PORTC = 0xFF;

	for(int i = 0 ; i<4; i++)
	{
		PORTB = ~(0x01<<i);
		PORTC = ~(0x08>>i);
		_delay_ms(200);
	}
	for(int i = 0; i<3; i++)
	{
		PORTB = ~(0x04>>i);
		PORTC = ~(0x02<<i);
		_delay_ms(200);
	}
}

void disp(num)
{
	//if(num%16 == 0)
	
		PORTD = (s[num/16]<<4&0x30) | (s[num%16]<<6&0xC0);
		PORTB = (s[num/16]>>2&0x07) | (s[num%16]<<1&0x38);
		PORTC = (s[num/16]>>5&0x07) | (s[num%16]>>2&0x38);
	
}

int main(void)
{
	DDRD = 0xF3;
	DDRB = 0x3F;
	DDRC = 0x3F;

	while(1)
	{
		for(int i = 0; i<256; i++)
		{
			disp(i);
			_delay_ms(200);
		}
		
	}
}
