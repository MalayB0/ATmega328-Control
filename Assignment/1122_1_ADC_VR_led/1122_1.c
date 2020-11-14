#include <avr/io.h>
#define F_CPU 16000000UL
#include <util/delay.h>
#include <avr/interrupt.h>

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

void LED_GO(unsigned int num)
{
	unsigned int a = (0xFFFF)>>(15 - (num/64));
	PORTD = ((a>>4) & 0x00F0) | (~(a>>6) & 0x000F);
	PORTB = ((a>>12) & 0x000F) | (~(a<<4) & 0x00F0);
	PORTC = (PORTC & 0x01) | (~(a>>1) & 0x00FE);
}

ISR(INT0_vect)
{
	cli();
	
	unsigned int value;
	value = GetADCData(0);
	LED_GO((unsigned char)value);

	EIFR |= 0x01;
	sei();
}

int main ()
{
	cli();
	DDRD = 0XF3;
	DDRB = 0XFF;
	DDRC = 0xFE;

	LED_GO(0);

	EICRA = 0X0A;
	// falling
	EIMSK = 0X01;
	// interrupt enable
	EIFR |= 0X01;
	
	unsigned int value;
		
	sei();

	while(1)
	{
		value = GetADCData(0);
		LED_GO((unsigned int)value);
	}
}
