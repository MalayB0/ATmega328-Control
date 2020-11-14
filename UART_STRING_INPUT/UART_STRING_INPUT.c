#include<avr/io.h>
#define F_CPU 16000000UL
#include <util/delay.h>
#include <avr/interrupt.h>
#define   BAUD_9600   103
#define MAX_BUFF_SIZE   128

#include "Printf.h"
#include "LED_ALL_ON_OFF.h"
#include "LED_ALTERNATING.h"
#include "LED_SHIFT.h"

unsigned char _buff[MAX_BUFF_SIZE];
unsigned char com_size = 0;
unsigned char sint_flag;



// termination character

ISR(USART_RX_vect)
{
   unsigned char _usart_rcv = 0;
   _usart_rcv = UDR0;
   if(_usart_rcv == 'i' || com_size >= MAX_BUFF_SIZE)
   {
         sint_flag = 1;
   }
   else
   {
         _buff[com_size++] =_usart_rcv;
   }
   sei();
}


unsigned char command(uint8_t *buff, uint8_t len)
{
   unsigned char tmp[MAX_BUFF_SIZE];
   unsigned char ret = 0;
   memset(tmp,'\0',MAX_BUFF_SIZE);
   // pointer ÀØÁö¾Ê±â
   memcpy((uint8_t*)tmp,(uint8_t*)buff,len);
   if(strstr((char*)tmp,"LED_ALL_ON_OFF") != NULL){
      LED_ALL_ON_OFF(3);
      ret = 1;
   }else if(strstr((char*)tmp,"LED_ON_OFF_ALT") != NULL){
        LED_ALTERNATING(3);
      ret = 1;
   }else if(strstr((char*)tmp,"LED_ALL_SHIFT") != NULL){
      LED_SHIFT(3);
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
   DDRC = 0xFE;
   DDRB = 0xFD;
   uint8_t buf[MAX_BUFF_SIZE];
   unsigned char Rx_char = 0;
   cli();
   UBRR0H = (unsigned char) (BAUD_9600 >>8);
   UBRR0L = (unsigned char) BAUD_9600;
   UCSR0A = 0X00; 
   UCSR0B = (1<< TXEN0)|  (1<< RXEN0); // defalt  
   UCSR0B |=   (1<< RXCIE0);
   UCSR0C = (1<< UCSZ01) | (1<< UCSZ00);   

   sei();
   while(1){
         if(sint_flag){
            _buff[com_size] = '\0';
            
            if(!command(_buff,com_size))
            Printf("%s", _buff);
            
            com_size = 0;
            memset(_buff,'\0',128);
            sint_flag = 0;
         }
   }
}
