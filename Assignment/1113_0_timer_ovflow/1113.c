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

ISR(TIMER0_OVF_vect)
{
   TCNT0 = 0XFF - 0X4E; //To make 5ms
   TovVal++;

   if(TovVal == 200)
   {
      LED_BLINK();
      TovVal = 0;
   }

   sei();
}

int main()
{
   unsigned char in;
   cli();
   DDRD = 0XFF;
   PORTD = 0XFF;

   TCCR0A = 0X00;
   //NORMAL MODE

   TCCR0B |= (1<<CS02);
   //1024 prescaler
   TCCR0B |= (1 << CS00);

   TCNT0 = 0XFF - 0X4E;
   //To generate OV at every 5ms

   TIMSK0 |= 1 << TOIE0;
   //Timer0 overflow interrupt enable
   TIFR0 |= 1 << TOV0;

   sei();

   while(1)
   {}
}
