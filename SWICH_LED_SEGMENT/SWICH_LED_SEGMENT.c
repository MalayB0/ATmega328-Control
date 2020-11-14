#include<avr/io.h>
#define F_CPU 16000000UL
#include<util/delay.h>
#include<avr/interrupt.h>
#define INT0_SW 1
#define INT1_SW 2

unsigned char segD[] = {0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x27,0x7F,0x6F,0x77,0x7C,0x39,0x5E,0x79,0x71};
unsigned char in = 0;
int cnt = 0;


int disp_seg(int c)
{
	PORTD = ((segD[c]&0x0F)<<4);
	PORTB = ((segD[c]&0xF0)>>4)|0x30;
}

int LED_SHIFT()
{
	PORTB = (PORTB|0xF0);
	unsigned char REALB;
	for(int i=0;i<3;i++)
	{
		_delay_ms(100);
		disp_seg(cnt);
		REALB = PORTB;
		PORTB = REALB & (~(0x10<<i));
	}
	for(int i=0;i<6;i++)
	{
		REALB = PORTB;
		PORTC = ~(0x01<<i);
		_delay_ms(100);
	}
	
	for(int i=0;i<6;i++)
	{
		_delay_ms(100);
		REALB = PORTB;
		PORTC = ~(0x10>>i);
	}
	for(int i=0;i<2;i++)
	{
		disp_seg(cnt);
		REALB = PORTB;
		PORTB = REALB & (~(0x20>>i));
		_delay_ms(100);
	}
}


ISR(INT0_vect)
{
	cli();
	EIFR|=0x01;
	cnt = ++cnt%16;
	disp_seg(cnt);
	in = INT0_SW;

	sei();
	
}

ISR(INT1_vect)
{
	cli();
	EIFR|=0x02;
	in = INT1_SW;
	sei();
}

void main()
{
	cli();
	DDRD = 0xF3;
	DDRB = 0xFF;
	DDRC = 0xFF;
	PORTC = 0xFF;

	EICRA = 0x0F;
	EIMSK = 0x03; 
	EIFR = 0x03; 
	sei();

	while(1)
	{
		if(in == INT1_SW)
		{
			in = 0;
			for(int i = cnt;i>-1;i--)
			{
				if(in == INT0_SW)
				{
					i = cnt;
					break;
				}
				if(in == INT1_SW)
				{
					break;
				}
				if(cnt != 0)
				{
					LED_SHIFT();
					cnt--;
				}
			}
			in = 0;
		}
		disp_seg(cnt);
		_delay_ms(200);
	}

}
