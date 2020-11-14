#include<avr/io.h>
#define F_CPU 16000000UL
#include<util/delay.h>

unsigned char seg[] = {0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x27,0x7F,0x6F};

int segmentcontrol()
{
	for(int i = 0; i<100;i++)
	{
		PORTD = seg[i/10]<<4;
		PORTB = ((seg[i%10]<<4)&0x30)|(seg[i/10]>>4);
		PORTC = seg[i%10]>>2;
		_delay_ms(200);
	}
	for(int i = 99; i>-1;i--)
	{
		PORTD = seg[i/10]<<4;
		PORTB = ((seg[i%10]<<4)&0x30)|(seg[i/10]>>4);
		PORTC = seg[i%10]>>2;
		_delay_ms(200);
		
	}
}

int main()
{
	while(1)
	{
		DDRD = 0xFF;
		DDRB = 0xFF;
		DDRC = 0xFF;
		segmentcontrol();
	}
}
