#include <avr/io.h>
#define F_CPU 16000000UL
#include <util/delay.h>
#include <avr/interrupt.h>
#include <math.h>
#include <stdio.h>
#define FREQ(x) (unsigned int)(16000000/(2*8*(1+x)))
#define BAUD_9600      103

#include "LED_ALTERNATING.h"
#include "LED_ALL_ON_OFF.h"
#include "LED_SHIFT.h"
#include "SEG_UPCOUNTING.h"
#include "Printf.h"
#include "BUZZ.h"

#define FREQ(x) (unsigned int)(16000000/(2*8*(1+x)))
#define   LEDONOFF   0x01
#define   LEDALT    0X02   
#define   LEDSHIFT   0X04
#define SEGMENT_UPCOUNTING 0x08
#define BUZZER       0x10  
#define DOMISOL		0x05
#define   STX   0X02
#define   ETX   0X03
#define ON 1
#define OFF 0

unsigned char Rx_char = 0;
int strlenth = 0;
int flag = 0;
int sint_flag = 0;
unsigned char cnt = 0;
unsigned char _command = 0;


ISR(USART_RX_vect){
   unsigned char _usart_rcv = 0;
         
   _usart_rcv = UDR0;

   if(flag == 0 && _usart_rcv == STX)
   {
      flag =1;
   }
   else if(flag ==1)
   {
      _command = _usart_rcv;
      flag = 2;
   }
   else if(flag ==2)
   {
      cnt = _usart_rcv;
      flag = 3;
   }
   else if(flag == 3)
   {
      if(_usart_rcv == ETX)
      {
         sint_flag = 1;
      }
      else
      {
         Printf("RX error\n");
      }
      flag = 0;
   }
   else
   {
      Printf("RX error\n");
   }
   sei();
}

void domisol(int a)
{
	for(int i=0;i<a;i++)
	{
		play(FREQ(523));
		_delay_ms(1000);
		play(FREQ(659));
		_delay_ms(1000);
		play(FREQ(783));
		_delay_ms(1000);
		stop();
	}
}

void hex_command(unsigned char cd)
{

   if(cd == LEDONOFF){
      LED_ALL_ON_OFF(cnt);
   }else if(cd == LEDALT){
       LED_ALTERNATING(cnt);
   }else if(cd == LEDSHIFT){
      LED_SHIFT(cnt);
   }else if(cd == SEGMENT_UPCOUNTING){
      SEG_UPCOUNTING(cnt);
   }else if(cd == DOMISOL)
   {
   		domisol(cnt);
   }
   else if(cd == BUZZER){
   		while(cnt == ON){
			play(1000);
		}
		if(cnt == OFF){
			stop();
			Printf("stop");
		}
   }
   else{
      Printf("Invalid Command\n");
   }
}



int main()
{
   cli();
   DDRD = 0xF0;
   DDRB = 0xFD;
   DDRC = 0xFF;
   EICRA = 0xAA;
   EIMSK = 0xFF;
   EIFR |= 0x0F;

   UBRR0H = (unsigned char)(BAUD_9600>>8);
   UBRR0L = (unsigned char)BAUD_9600;
   UCSR0B = (1<<TXEN0)|(1<<RXEN0);
   UCSR0B |=(1<< RXCIE0);
   UCSR0C = (1<<UCSZ01)|(1<<UCSZ00);
   sei();
   while(1)
   {
       if(sint_flag==1)
       {    
         hex_command(_command);
         sint_flag = 0;
         _command = 0;
       }
   }
}
