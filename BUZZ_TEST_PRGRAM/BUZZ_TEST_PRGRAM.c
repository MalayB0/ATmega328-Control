#include<avr/io.h>
#define F_CPU 16000000UL
#include <util/delay.h>
#include<avr/interrupt.h>
unsigned char TovVal = 0;
#define FREQ(x) (unsigned int)(16000000/(2*8*(1+x)))
signed int note_freq[3] = {523, 659, 783};
int note = 0;

void play(int y)
{
   DDRB |= 0x02;
   TCCR1A |= 0x40;
   TCCR1B |= 0x1A;
   ICR1 = FREQ(note_freq[y]);
}

void stop(void)
{
   TCCR1A |= 0x00;
   TCCR1B |= 0x00;
   TCNT1 = 0;
   ICR1 = 0;
   DDRB = ~0x02;
}



int main()
{
   unsigned char in;
   DDRD = 0XFF;
   PORTD = 0xFF;

   while(1){ 
      play(0);
      _delay_ms(1000);
      play(1);
      _delay_ms(1000);
      play(2);
      _delay_ms(1000);
   }
}
