#include <avr/io.h>
#define F_CPU 16000000UL
#include <util/delay.h>
#include <avr/interrupt.h>
#include <math.h>

// P248
unsigned int GetADCData(uint8_t aIn)
{
	volatile unsigned int result;
	ADMUX = aIn;
	ADCSRA = (1<<ADEN)|(1<<ADPS2)|(1<<ADPS1);
	ADCSRA |= (1<<ADSC);
	while(!(ADCSRA&(1<<ADIF)));
	//starting converser
	//ADIF가 셋이 될때까지 기다린다 
	result = ADCL + (ADCH<<8); // 16비트는 그대로, 8비트는 상위값으로 옮겨줌
	_delay_ms(1);
	ADCSRA = 0x00;
	return result;

}

//LED : PB4,PD0~1,PC1~5
//세그먼트 : PD4~7,PB0~3
void LED_SEG_ON(int input)
{
	int in = input/63;
	if(input == 0)
	{
		PORTB = 0xFF&~((0xFFFF<<(in-11))&0x0F);
		PORTD = (0xF0|(PORTD&0x0C)|((0xFFFF<<in)&0x03))&~((0xFFFF<<(in-3))&0xF0);
		PORTC = (PORTC&0x01)|((0xFFFF<<(in-1))&0x3E);
	}
	else
	{
		PORTB = 0xEF&~((0xFFFF<<(in-11))&0x0F);
		PORTD = (0xF0|(PORTD&0x0C)|((0xFFFF<<in)&0x03))&~((0xFFFF<<(in-3))&0xF0);
		PORTC = (PORTC&0x01)|((0xFFFF<<(in-1))&0x3E);
	}
}

int main()
{
	cli();
	DDRD = 0xF3;
	DDRB = 0xFF; 
	DDRC = 0x3E;   
	EICRA = 0x0A;
	//Falling Edge Triger
	EIMSK = 0x03;
	//interrupt enable
	EIFR |=0x03;

	sei();
	
	while(1)
	{
		LED_SEG_ON(((unsigned int)GetADCData(0)));
	}
}
