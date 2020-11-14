#include<avr/io.h>
#define F_CPU 16000000UL
#include<util/delay.h>
#include<avr/interrupt.h>


#define INT0_SW 1
#define INT1_SW 2

unsigned char segD[10] = {0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x27,0x7F,0x6F};

unsigned char in;

int cnt=0;

void disp_seg(int c);
void LED_SHIFT();

void disp_seg(int c)
{
	PORTD = ((segD[c]&0x0F)<<4);
	PORTB = ((segD[c]&0xF0)>>4)|0x30;
}

void LED_SHIFT()
{
	PORTB = (PORTB|0xF0);
	unsigned char REALB = PORTB;

		for(int i=0;i<3;i++)
		{
			_delay_ms(100);
		//	REALB = PORTB;
			PORTB = REALB & (~(0x10<<i));
		}
		REALB = PORTB;
		for(int i=0;i<6;i++)
		{
		//	REALB = PORTB;
			PORTC = ~(0x01<<i);
			_delay_ms(100);
		}
		REALB = PORTB;
		for(int i=0;i<6;i++)
		{
			_delay_ms(100);
		//	REALB = PORTB;
			PORTC = ~(0x10>>i);
		}
		REALB = PORTB;
		for(int i=0;i<2;i++)
		{
		//	REALB = PORTB;
			PORTB = REALB & (~(0x20>>i));
			_delay_ms(100);
		}

}


ISR(INT0_vect)
{
	cli();
	EIFR |=0x01;
	++cnt;
	cnt = cnt%10;
	disp_seg(cnt);
	in = INT0_SW;

	sei();
	
}

ISR(INT1_vect)
{
	cli();
	EIFR |=0x02;
	in = INT1_SW;
	sei();
}

int main()
{
	cli();

	int i;

	DDRD = 0xF3;
	DDRB = 0xFF;
	DDRC = 0xFF;
	PORTC = 0xFF;

	EICRA = 0x0F; // ¼Â ÀÎÅÍ·´Æ® ¶óÀÌÂ¡¿§Áö
	EIMSK = 0x03; //
	EIFR = 0x03; //

	sei();

	while(1)
	{

		if(in == INT1_SW)
		{
			in = 0;
			for(i = cnt;i>0;i--)
			{
				if(in == INT0_SW)
				{
					//break;
				}
				if(in == INT1_SW)
				{
					disp_seg(i);
					//break;
				}
				LED_SHIFT();
				disp_seg(i-1);
			}
			cnt = i;
			disp_seg(cnt);
			in = 0;
		//	_delay_ms(200);
		}
		disp_seg(cnt);
	}

}
