#include <avr/io.h>
#define F_CPU 16000000UL
#include <util/delay.h>
#include <avr/interrupt.h>
#define ON 1
#define OFF 0
#define FREQ(x) (unsigned int)(16000000/(2*8*(1+x)))
#include <math.h>

signed int note_freq[7] = {440, 493, 523, 587, 659, 698, 783};
signed int note_freqmin[7] = {783, 698, 659, 587, 523, 493, 440};

int on_offup = 0;
int on_offdown = 0;
int note = 0;
int flag = 0;

void play(signed int y)
{
	DDRB |= 0x02;
	TCCR1A |= 0x40;
	TCCR1B |= 0x1A;
	if(y<0)
	{
		ICR1 = FREQ(note_freqmin[-y%7]*pow(0.5,-y/7+1));
	}
	else
	{
		ICR1 = FREQ(note_freq[y%7]*pow(2,(y/7)));
	}
}

void stop(void)
{
	TCCR1A |= 0x00;
	TCCR1B |= 0x00;
	TCNT1 = 0;
	ICR1 = 0;
	DDRB = ~0x02;
}

void LED_OFF(void)
{
	PORTC = 0xFF;
	PORTB = 0xFD;
}

void LED_SHIFT(void)
{
	if(note < 0)
	{
		int notemin = note+(7*((-note/7)+1));
		if(notemin%7 < 4)
		{
   			PORTB = (PORTB|0x3C) & ~(0x04 << (notemin%7));
			PORTC = 0x0F;
 		}
		else if(notemin%7 > 3)
		{
			PORTB = PORTB|0x3C;
			PORTC = ~(0x01<<((notemin%7)-4));
		}
	}
	if(note%7 < 4 && note>=0)
	{
   		PORTB = (PORTB|0x3C) & ~(0x04 << (note%7));
		PORTC = 0x0F;
 	}
	else if(note%7 > 3)
	{
		PORTB = PORTB|0x3C;
		PORTC = ~(0x01<<((note%7)-4));
	}
}

ISR(INT0_vect)
{
	cli();
	on_offup = ON;
	++note;
	EIFR |= 0x01;
	sei();
}

ISR(INT1_vect)
{
	cli();
	on_offdown = ON;
	EIFR |= 0x02;
	--note;
	sei();
}

int main()
{
	cli();
	DDRD = 0xF3;
	DDRB = 0xFF; // 하위 4비트 활성화
	DDRC = 0xFF;   
   
	PORTC = 0xFF;
	PORTD = 0x0C; //PB 2~5이랑 PC0~3 사용 예정
	PORTB = 0xFD;
	EICRA = 0x0F;
	//Falling Edge Triger
	EIMSK = 0x03;
	//interrupt enable
	EIFR |=0x03;

	sei();
	while(1)
	{
		LED_SHIFT();
		flag = PIND;
		if(flag ==  0x08)
		{
			on_offdown = OFF;
		}
		if(flag == 0x0C && on_offdown == OFF && on_offup == ON)
		{
			--note;
			on_offup = OFF;
		}
		if(on_offup == ON && flag == 0x0C && on_offdown == ON)
		{
			++note;
			while(1)
			{
				if(on_offup == ON)
				{
					on_offdown = OFF;
					LED_SHIFT();
					play(note);
					flag = PIND;
					if(flag == 0x0C)
					{
						on_offdown = OFF;
						LED_SHIFT();
						play(note);
					}
				}
				if(on_offdown == ON)
				{	
					on_offup = OFF;
					LED_SHIFT();
					play(note);
					flag = PIND;
					if(flag == 0x0C)
					{
						--note;
						LED_SHIFT();
						on_offup = OFF;
						on_offdown = OFF;
					}
				}
				if(on_offup == OFF && on_offdown == OFF)
				{
					stop();
					break;
				}
			}
		}
		stop();
	}
}
