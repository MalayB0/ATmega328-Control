#include<avr/io.h>
#define F_CPU 16000000UL
#include<util/delay.h>

unsigned char segD[10] = {0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x27,0x7F,0x6F};


int disp_seg(int n)
{
	PORTD = (segD[n]&0x0F)<<4;
	PORTB = (segD[n]&0xF0)>>4;
}

int main()
{
	unsigned char in;

	DDRD = 0xF3;
	DDRB = 0xFF;
	while(1)
	{
		in = ((PIND&0x0C)>>2);
		disp_seg(in);
		//in = ~(PIND&0x0C);
		//PORTB = in;
	}
}
