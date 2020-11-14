#include<avr/io.h>
#define F_CPU 16000000UL
#include <util/delay.h>
#include <avr/interrupt.h>
#define	BAUD_9600	103
#define MAX_BUFF_SIZE	128

#include "Printf.h"
#include "LED_ALL_ON_OFF.h"
#include "LED_ON_OFF_ALT.h"
#include "LED_ALL_SHIFT.h"
#include "SEG_UPCOUNTING.h"
#include "BUZZER_ON_OFF.h"

#define LED_ALL_ON_OFF	0x01
#define LED_ALL_SHIFT	0x02
#define LED_ON_OFF_ALT	0x04
#define SEG_UPCOUNTING	0x08
#define BUZZER_ON_OFF	0x0F
#define STX	0x02
#define ETX 0x03

int sint_flag = 0;
int decode = 0;
unsigned char _command = 0;

int _delay_time = 0;
unsigned char count = 0;

// termination character

ISR(USART_RX_vect)
{
	unsigned char _usart_rcv = 0;
	_usart_rcv = UDR0;

	if(decode == 0 && _usart_rcv == STX){
		decode = 1;
	}else if(decode == 1){
		_command = _usart_rcv;
		decode = 2;
	}else if(decode == 2){
		count = _usart_rcv;
		decode = 3;
	}else if(decode == 3){
		if(_usart_rcv == ETX){
			sint_flag = 1;
		}else{
			Printf("RX error\n");
		}
		decode = 0;
	}
	sei();
}

void hex_command(uint8_t cd){
	
	if(cd == LED_ALL_ON_OFF){
		led_all_on_off(3,400);
	}else if(cd == LED_ALL_SHIFT ){
		led_all_shift(3);
	}else if(cd == LED_ON_OFF_ALT){
		led_on_off_alt(3,400);
	}else{
		Printf("Invalid Command\n");
	}

}

void quad_hex_command(unsigned char cd, unsigned char cnt){
	if(cd == LED_ALL_ON_OFF){
		led_all_on_off(cnt,400);
	}else if(cd == LED_ALL_SHIFT){
		led_all_shift(cnt);
	}else if(cd == LED_ON_OFF_ALT){
		led_on_off_alt(cnt,400);
	}else if(cd == SEG_UPCOUNTING){
		seg_upcounting(cnt);
	}else if(cd == BUZZER_ON_OFF){

						while(count == ON){
							play(1000);
							Printf("1");
							Printf("a");
						}
						if(count == OFF){
							stop();
							Printf("wrong");
						}
	}
	/*
	else if(cd == BUZZER_ON_OFF){
		if(cnt == ON)
		{
			while(cnt != OFF)
			{
				play(1000);
			}
		}
		if(cnt == OFF)
		{
			Printf("stop enable\n");
			stop();
		}
		if(cnt != ON && cnt != OFF)
		{
			Printf("commend BUZZ DATA is only 0x01 or 0x00\n");
		}
	}*/
	else{
		Printf("Inavalid Command\n");
	}
}

unsigned char command(uint8_t *buff, uint8_t len)
{
	unsigned char tmp[MAX_BUFF_SIZE];
	unsigned char ret = 0;
	memset(tmp,'\0',MAX_BUFF_SIZE);
	// pointer ÀØÁö¾Ê±â
	memcpy((uint8_t*)tmp,(uint8_t*)buff,len);
	if(strstr((char*)tmp,"LED_ALL_ON_OFF") != NULL){
		led_all_on_off(3,400);
		ret = 1;
	}else if(strstr((char*)tmp,"LED_ON_OFF_ALT") != NULL){
	  	led_on_off_alt(3,400);
		ret = 1;
	}else if(strstr((char*)tmp,"LED_ALL_SHIFT") != NULL){
		led_all_shift(3);
		ret = 1;
	}else{
		Printf("Your initial is not correct, that is: ");
		ret = 0;
	}
	return ret;
}
int main()
{
	DDRD = 0x30;
	DDRC = 0xFF;
	uint8_t buf[MAX_BUFF_SIZE];
    unsigned char Rx_char = 0;
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
				//hex_command(_command);
				quad_hex_command(_command,count);
				sint_flag = 0;
				_command = 0;
				count = 0;
			}
	}
} 

