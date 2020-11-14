#include <avr/io.h>
#define F_CPU 16000000UL
#include <util/delay.h>
#include <avr/interrupt.h>
#include <math.h>
unsigned char Seg_Arr[16] = {0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x27,0x7F,0x6F,0x77,0x7C,0x39,0x5E,0x79,0x71};

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

segmentdsp(unsigned char input)
{
	unsigned int left = input/16;
	unsigned int right =input%16;
	PORTD = (Seg_Arr[left]<<4)|(Seg_Arr[right]>>5);
	PORTB = (Seg_Arr[left]>>4)|((Seg_Arr[right]>>3)&0x10);
	PORTC = (PORTC&0x01)|(Seg_Arr[right]<<1);
}


ISR(INT0_vect)
{
	cli();
	unsigned int value = GetADCData(0);
	segmentdsp((unsigned char)value);
	EIFR |= 0x01;
	sei();
}

ISR(INT1_vect)
{
	cli();
	EIFR |= 0x02;
	sei();
}

int main()
{
	cli();
	DDRD = 0xF3;
	DDRB = 0xFF; // 하위 4비트 활성화
	DDRC = 0xFE;   

	segmentdsp((unsigned char)GetADCData(0));
	EICRA = 0xAA;
	//Falling Edge Triger
	EIMSK = 0x03;
	//interrupt enable
	EIFR |=0x03;

	sei();
	
	while(1)
	{
		segmentdsp((unsigned char)GetADCData(0));
	}
}
