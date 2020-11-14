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

int SW1 = 0;
int SW2 = 0;
int PSC1 = 0; // Play, Stop control
int PSC2 = 0;
int beep = 0;

/*libc.a 의 추가 ->컴파일 오류 해결
  play 함수 안의 do_while문 제거 -> 소리 오류 해결
  7+(nt%7))%7 : 마이너스 */

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

void LED_note(unsigned int num)
{
	PORTD = ~((1<<note%7)<<4);
	PORTC = ~((1<<note%7)>>2);
}

/*interrupt1---------------------------------------------------------*/

ISR(INT0_vect)
{
	cli();

	if(SW1 == ON)	// 1번 스위치 땜.
	{
		if(PSC1 == OFF)
		{
			if(note <44 && SW2 == OFF)
			{
				note++;
				LED_note(note);
			}
			EICRA = 0X0F; //Rising edge 로 바꿈
			SW1 = OFF;	// 1번 스위치 끔.
			SW2 = OFF;	// 2번 스위치 끔.
		}
		PSC2 = OFF;
	}
	else if(SW1 == OFF)// 1번 스위치 눌렀음.
	{
		if(SW2 == ON)
		{
			beep = OFF;
			SW1 = ON;
			PSC1 = ON;
		}
		else if(SW2 == OFF)
		{
			EICRA = 0X0A; //Falling edge 로 바꿈
			SW1 = ON;
		}
	}

	EIFR |= 0x01;
	sei();
}

/*interrupt2---------------------------------------------------------*/

ISR(INT1_vect)
{
	cli();

	if(SW2 == ON)
	{
		if(PSC2 == OFF)
		{
			if(note > 0 && SW1 == OFF)
			{
				note--;
				LED_note(note);
			}
			EICRA = 0X0F; //Rising edge 로 바꿈
			SW1 = OFF;
			SW2 = OFF;
		}
		PSC1 = OFF;
	}
	else if(SW2 == OFF)
	{
		if(SW1 == ON)
		{
			beep = ON;
			SW2 = ON;
			PSC2 = ON;
		}
		else if(SW1 == OFF)
		{
		EICRA = 0X0A; //Falling edge 로 바꿈
		SW2 = ON;
		}
	}

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
	EICRA = 0X0F;
	//Rising Edge Triger
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

