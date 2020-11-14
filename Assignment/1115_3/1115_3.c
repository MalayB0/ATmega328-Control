#include <avr/io.h>
#define F_CPU 16000000UL
#include <util/delay.h>
#include <avr/interrupt.h>

#define FREQ(x) (unsigned int)(16000000/(2*8*(1+x)))

#include <math.h>

#define ON  1
#define OFF 0

unsigned int note_freq[7] = {440,493,523,587,659,698,783};
int note = 0;

unsigned int beep = 0;
unsigned int sw1 = 0;
unsigned int sw2 = 0;

/*libc.a 의 추가 ->컴파일 오류 해결
  play 함수 안의 do_while문 제거 -> 소리 오류 해결
  (7+(nt%7))%7 : 마이너스 
  0.5,-y/7+1 */

void play(int nt)
{
	DDRB |= 0X02;
	TCCR1A |= 0X40;
	TCCR1B |= 0X1A;
	ICR1 = FREQ(note_freq[nt%7]*pow(2,(nt/7)));
}

void stop(void)
{
	DDRB = ~0X02;
	TCCR1A |= 0X00;
	TCCR1B |= 0X0A;
	TCNT1 = 0;
	ICR1 = 0;
}

void LED_note(int num)
{
	PORTD = ~((1<<num%7)<<4);
	PORTC = ~((1<<num%7)>>2);
}

/*interrupt1---------------------------------------------------------*/

ISR(INT0_vect)
{
	cli();
	
	if((PIND & 0x0C) == 0x08)
	{
		beep = OFF;
		sw1 = ON;
	}
	else if(note <44 && sw2 == OFF)
	{
		note++;
		LED_note(note);
	}
	sw2 = OFF;

	EIFR |= 0x01;
	sei();
}

/*interrupt2---------------------------------------------------------*/

ISR(INT1_vect)
{
	cli();

	if((PIND & 0x0C) == 0x04)
	{
		beep = ON;
		sw2 = ON;
	}
	else if(note >0 && sw1 == OFF)
	{
		note--;
		LED_note(note);
	}
	sw1 = OFF;

	EIFR |= 0x02;
	sei();
}

int main(void)
{
	cli();
	DDRD = 0XF3;
	DDRB = 0X00; // 모두 입력으로...
	DDRC = 0xFF;

	PORTD = 0XE0;
	PORTB = 0X00;
	PORTC = 0xFF;
	EICRA = 0X0A;

	EIMSK =0X03;
	// interrupt enable
	EIFR |= 0X03;
	
	sei();
	while(1)
	{
		while(beep == ON)
		{
			play(note);
		}
		stop();
	}
}

