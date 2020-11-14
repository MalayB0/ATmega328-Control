/*
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define F_CPU 16000000UL
#define FREQ(x) (unsigned int)(16000000/(2*8*(1+x)))

#define ON  1
#define OFF 0

unsigned int note_freq[7] = {440,493,523,587,659,698,783}

int on_off = 0;

void play(unsigned int y)
{
	unsigned int x;
	DDRB |= 0X02;
	TCCR1A |= 0X40;
	TCCR1B |= 0X1A;
	ICR1 = y;
}

void stop(void)
{
	TCCR1A |= 0X00;
	TCCR1B |= 0X0A;
	TCNT1 = 0;
	ICR1 = 0;
	DDRB = ~0X02;
}

ISR(INT0_vect)
{
	cli();
	on_off = ON;
	EIFR |= 0x01;

	sei();
}

ISR(INT1_vect)
{q
	cli();
	on_off = OFF;
	EIFR |= 0x02;

	sei();
}

int main()
{
	cli();
	DDRD = 0XF3;
	DDRB = 0X0F; // 하위 4비트 활성화

	PORTD = 0X00;
	PORTB = 0X00;
	EICRA = 0X0A;
	//Falling Edge Triger
	EIMSK =0X03;
	// interrupt enable
	EIFR |= 0X03;
	
	sei();
	while(1)
	{
		if(on_off == ON)
		{
			play(2273); // 라에 해당하는 OCR(ICR) 값
		}
		else
		{
			stop();
		}
	}
}

*/


#include <avr/io.h>
#define F_CPU 16000000UL
#define FREQ(x) (unsigned int)((16000000/(2*8*x))-1)
#include <util/delay.h>
#include <avr/interrupt.h>
#include <math.h>

/*#define FREQ(x) (unsigned int)(16000000/(2*8*(1+x)))*/


unsigned int note_freq[7] = {440,493,523,587,659,698,783};
unsigned int note = 0;

int on_off = 0;
int a;

void play(unsigned int note)
{
	unsigned int x, y;
	TCCR1A |= 0X40;
	TCCR1B |= 0X1A;
	a = note/7;
	y = FREQ(note_freq[note%7]*pow(2,a));
	do
	{
		x = TCNT1;
	}while(x>=y);
	ICR1 = y;
}


ISR(INT0_vect)
{
	cli();
	note++;
	PORTD = ~((1<<note%7)<<4);
	PORTC = ~((1<<note%7)>>2);
	EIFR |= 0x01;

	sei();
}

ISR(INT1_vect)
{
	cli();
	/*if(x==0)
	{
		x = 0x40;
	}*/
	if(note!=0)
	{
		note--;
		PORTC = ~((1<<note%7)>>2);
		PORTD = ~((1<<note%7)<<4);
		EIFR |= 0x02;
	}

	sei();
}

int main(void)
{
	cli();
	DDRD = 0XF3;
	DDRB = 0X0F; // 하위 4비트 활성화
	DDRC = 0xFF;

	PORTD = 0XE0;
	PORTB = 0X00;
	PORTC = 0xFF;
	EICRA = 0X0A;
	//Falling Edge Triger
	EIMSK =0X03;
	// interrupt enable
	EIFR |= 0X03;
	
	sei();
	while(1)
	{
		play(note); // note 값
	}
}
