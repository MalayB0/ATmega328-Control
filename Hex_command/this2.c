#include <avr/io.h>
#define F_CPU 16000000UL
#include<util/delay.h>
#include <avr/interrupt.h>
#define BAUD_9600      103
#define	MAX_BUFF_SIZE	128
unsigned char Rx_char = 0;
unsigned char sint_flag;
unsigned int note_freq[7] = {440, 493, 523, 587, 659, 698, 783};
int decode = 0;
unsigned char  _command = 0;
int count = 0;


#include "Printf.h"
#include "LED_ALL_ON_OFF.h"
#include "LED_ANTERNATING_ON_OFF.h"
#include "LED_SHIFT.h"
#include "SEGMENT_UPCOUNTING.h"
#include "BUZZER_ON_OFF.h"

#define   LEDALLONOFF 		     	0x01
#define   LEDANTERNATINGONOFF       0x02   
#define   LEDSHIFT   		        0x04
#define   SEGMENTUPCOUNTING 		0x08
#define   BUZZER					0x10
#define   STX  					    0x02
#define   ETX   					0x03


void hex_command(unsigned char cd)
{
	if(cd == LEDALLONOFF){
	LED_ALL_ON_OFF(count);
	}else if(cd == LEDANTERNATINGONOFF){
	LED_ANTERNATING_ON_OFF(count);
	}else if(cd == LEDSHIFT){
	LED_SHIFT(count);
	}else if(cd == SEGMENTUPCOUNTING){
	SEGMENT_UPCOUNTING(count);
	}else if(cd == BUZZER){
	BUZZER_ON_OFF(count);
	}else{
	Printf("Invalid Command\n");
	}
}

ISR(USART_RX_vect){
	unsigned char _usart_rcv = 0;
         
	_usart_rcv = UDR0;

	if(decode == 0 && _usart_rcv == STX){
	decode =1;
	}else if(decode==1){
	_command = _usart_rcv;
	decode = 2;
	}else if(decode==2){
	count = _usart_rcv;
	decode = 3;
	}else if(decode==3){
		if(_usart_rcv == ETX){
		sint_flag = 1;
		}else{
		Printf("RX error\n");
		}
	decode = 0;
	}
	sei();
}


int main()
{
	DDRD = 0xF0;
	DDRC = 0xFF;
	DDRB = 0xCC;
	cli();
	UBRR0H = (unsigned char) (BAUD_9600 >>8);
	UBRR0L = (unsigned char) BAUD_9600;
	UCSR0A = 0X00;
	UCSR0B = (1<< TXEN0)|  (1<< RXEN0);   
	UCSR0B |=   (1<< RXCIE0);
	UCSR0C = (1<< UCSZ01) | (1<< UCSZ00);   

	sei();
	while(1){
		if(sint_flag){    
		hex_command(_command);
		sint_flag = 0;
		_command = 0; 
		}
	}
}
