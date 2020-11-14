#include<avr/io.h>
#define F_CPU 16000000UL
#include <util/delay.h>
#include <avr/interrupt.h>
#include <string.h>
#define	BAUD_9600	103
#define	MAX_TXBUF_SIZE	128
unsigned char tx_buf[MAX_TXBUF_SIZE];
int buf_len = 0;

void tx_char(unsigned char txChar)
{
      while(!(UCSR0A & (1<<UDRE0)));
	UDR0 = txChar;
}

void tx_str(unsigned char *txStr, int len)
{
	int i;
	for(i = 0;i< len; i++){
		tx_char(txStr[i]);
	}
}
ISR (INT0_vect)
{
	unsigned char *tmp;

	tmp = malloc(30);
	memset(tmp,'\0',30);
	tmp = "INT1 PUSHED\n";
	buf_len = strlen(tmp);
	strncpy(tx_buf,tmp,buf_len);
	EIFR |= 0x01; 
	sei();
}


int main()
{
	cli();
	DDRD = 0XF0;
	EICRA = 0xAA; 	
	EIMSK = 0x0F; 	
	EIFR |= 0x0F;   


	UBRR0H = (unsigned char) (BAUD_9600 >>8) ;
	UBRR0L = (unsigned char) BAUD_9600;
	UCSR0B = (1<< TXEN0)|  (1<< RXEN0);	
	UCSR0C = (1<< UCSZ01) | (1<< UCSZ00);
	sei();

	while(1){
    	if(buf_len > 0){
            tx_str(tx_buf,buf_len);
			memset(tx_buf,'\0',MAX_TXBUF_SIZE);
	        buf_len = 0;
			}
	}
}
