#include <avr/io.h>
#define F_CPU 16000000UL
#include <util/delay.h>
#include <avr/interrupt.h>
#define Vin(x) (unsigned int)((x/1024) * 5)

unsigned char seg_num[] = 
	{0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7C,0x07,0x7F,0x67,0x77,0x7C,0x39,0x5E,0x79,0x71};

unsigned int GetADCData(uint8_t aIn)
{
	unsigned int result;
	ADMUX = aIn;
	ADCSRA = (1<<ADEN)|(1<<ADPS2)|(1<<ADPS1);
	ADCSRA |= (1<<ADSC);
	while(!(ADCSRA & (1<<ADIF))); // while ADCSRA 이 ~~ 될 동안 기다려라...
	result = ADCL + (ADCH << 8);
	_delay_ms(1);
	ADCSRA = 0x00; // 초기화
	return result;
}

void Disp_Seg(int num)
{
	int a = num/16;
	int b = num%16;
	PORTD = (seg_num[a]<<4) | (seg_num[b]>>6);
	PORTB = (seg_num[a]>>4) | (seg_num[b]<<4);
	PORTC = (PORTC & 0x01) | ((seg_num[b]>>1) & 0xFE);
}

ISR(INT0_vect)
{
	cli();
	
	unsigned int value;
	value = GetADCData(0);
	Disp_Seg((unsigned char)value);

	EIFR |= 0x01;
	sei();
}

int main ()
{
	cli();
	DDRD = 0XF3;
	DDRB = 0XFF;
	DDRC = 0xFE;

	Disp_Seg(0);

	EICRA = 0X0A;
	// falling
	EIMSK = 0X01;
	// interrupt enable
	EIFR |= 0X01;	
	sei();

	while(1)
	{
		unsigned int value;
		value = GetADCData(0);
		Disp_Seg((unsigned char)value);
	}
}
