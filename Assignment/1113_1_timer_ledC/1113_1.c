#include<avr/io.h>
#define F_CPU 16000000UL
#include<util/delay.h>
#include<avr/interrupt.h>
unsigned char TovVal = 0;

void LED_BLINK(void)
{
   PORTD = 0X00;
   _delay_ms(300);
   PORTD = 0XFF;
}

ISR(TIMER1_OVF_vect)
{
   TCNT1 = 0XFFFF - 0X3D09; //To make 1s
   LED_BLINK();

   sei();
}

int main()
{
   unsigned char in;
   cli();
   DDRD = 0XFF;
   PORTD = 0XFF;

   TCCR1A = 0X00;
   //NORMAL MODE

   TCCR1B |= (1<<CS12);
   //1024 prescaler
   TCCR1B |= (1 << CS10);

   TCNT1 = 0XFFFF - 0X3D09;
   //To generate OV at every 1s

   TIMSK1 |= 1 << TOIE1;
   //Timer0 overflow interrupt enable
   TIFR1 |= 1 << TOV1;

   sei();

   while(1)
   {}
}
