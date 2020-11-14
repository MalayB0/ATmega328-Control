#include<avr/io.h>
#define F_CPU 16000000UL
#include<util/delay.h>
#include <avr/interrupt.h>
#define BAUD_9600		103
unsigned char Rx_char = 0;
int strlenth = 0;

void tx_char(unsigned char txChar)
{
	while(!(UCSR0A&1<<UDRE0));
	UDR0 = txChar;
}

void tx_str(unsigned char *txStr,int len)
{
	for(int i = 0;i<len;i++)
	{
		tx_char(txStr[i]);
	}
}
//INT1 PUSHED
//INT2 PUSHED

ISR(INT0_vect)
{
	unsigned char *send_char;
	send_char = malloc(15);
	send_char = "INT1 PUSHED.\n";
	strlenth = strlen(send_char);
	tx_str(send_char,strlenth);
	EIFR|=0x01;
	sei();
}
ISR(INT1_vect)
{
	unsigned char *send_char;
	send_char = malloc(15);
	send_char = "INT2 PUSHED.\n";
	strlenth = strlen(send_char);
	tx_str(send_char,strlenth);
	EIFR|=0x02;
	sei();
}

int main()
{
	cli();
	DDRD = 0xF0;
	EICRA = 0xAA;
	EIMSK = 0xFF;
	EIFR |= 0x0F;

	UBRR0H = (unsigned char)(BAUD_9600>>8);
	UBRR0L = (unsigned char)BAUD_9600;
	UCSR0B = (1<<TXEN0)|(1<<RXEN0);
	UCSR0C = (1<<UCSZ01)|(1<<UCSZ00);
	sei();
	while(1)
	{
		
	}
}
//tx enable rx enable 중요
//8비트로 set
//send_char를











