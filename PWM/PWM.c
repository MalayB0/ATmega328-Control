#include <avr/io.h>
#define F_CPU 16000000UL
#include <util/delay.h>
#include <avr/interrupt.h>
#define ON 1
#define OFF 0
#define FREQ(x) (unsigned int)(16000000/(2*8*(1+x)))
#include <math.h>

unsigned int note_freq[7] = {440, 493, 523, 587, 659, 698, 783};

int on_off_up = 0;
int on_off_down = 0;
int note = -1;
int in = 0;
int beep = 0;

void play(unsigned int y)
{
   unsigned int x = FREQ(note_freq[y%7]*pow(2,y/7));
   DDRB |= 0x02;
   TCCR1A |= 0x40;
   TCCR1B |= 0x1A;
   ICR1 = x;
}

void stop(void)
{
   TCCR1A |= 0x00;
   TCCR1B |= 0x00;
   TCNT1 = 0;
   ICR1 = 0;
   DDRB = ~0x02;
}

int LED_SHIFT()
{
   PORTC = ~(0x01 << note%7);
   if(note%7 == 6){
   PORTB = ~(0x40 >> 4);
   }
   else{
   PORTB = 0x0C;
   }
}

ISR(INT0_vect)
{
   cli();
   note = ++note;
   in = PIND&0x0C;
   if(in == 0x0C)
   {
         beep = OFF;
   }
   LED_SHIFT();
   on_off_up = ON;
   EIFR |= 0x01;
   sei();
}

ISR(INT1_vect)
{
   cli();
   note = --note;
   in = PIND&0x0C;
   if(in == 0x0C)
   {
         beep = ON;
   }
   LED_SHIFT();
   on_off_down = ON;
   EIFR |= 0x02;
   if(note<0){
   note = 0;
   }
   sei();
}

int main()
{
   cli();
   DDRD = 0xF3;
   DDRB = 0x0F; // 하위 4비트 활성화
   DDRC = 0xFF;   
   
   PORTC = 0xFF;
   PORTD = 0x00;
   PORTB = 0x0C;
   EICRA = 0x0F;
   //Falling Edge Triger
   EIMSK = 0x03;
   //interrupt enable
   EIFR |=0x03;

   sei();

   while(1){
      while(beep == ON)
      {
         play(note);
      }
      stop();
   }
}
