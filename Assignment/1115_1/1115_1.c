#include <avr/io.h>
#define F_CPU 16000000UL
#include <util/delay.h>
#include <avr/interrupt.h>

#define FREQ(x) (unsigned int)(16000000/(2*8*(1+x)))

#include <math.h>

unsigned int note_freq[7] = {440,493,523,587,659,698,783};
int note = 0;

int on_off = 0;

/*libc.a 의 추가 ->컴파일 오류 해결
  play 함수 안의 do_while문 제거 -> 소리 오류 해결*/

void play(unsigned int nt)
{
	unsigned int y;
	TCCR1A |= 0X40;
	TCCR1B |= 0X1A;
	y = FREQ(note_freq[nt%7]*pow(2,(nt/7)));
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
