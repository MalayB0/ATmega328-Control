#include<avr/io.h>
#define F_CPU 16000000UL
#include<util/delay.h>
#include<avr/interrupt.h>


#define INT0_SW 1
#define INT1_SW 2

unsigned char segD[] = {0x3F,0x06,0x5B,0x4F};

unsigned char in;

int cnt=0;

void disp_seg(int c)
{
   PORTD = ((segD[c]&0x0F)<<4);
   PORTB = ((segD[c]&0xF0)>>4)|PORTB&0xF0;
}

int LED_ALL_ON_OFF()
{
   unsigned char REALB = ((segD[cnt]&0xF0)>>4);
   PORTC = 0x00;
   PORTB = PORTB&0x0F | 0x00;
   _delay_ms(100);
   PORTC = 0xFF;
   PORTB = PORTB&0x0F | 0x30;
   _delay_ms(100);
}

ISR(INT0_vect)
{
   cli();
   EIFR |=0x01;
   cnt = (++cnt)%4;
   EICRA = 0X03|(cnt<<2);
   disp_seg(cnt);
   in = INT0_SW;
   sei();
   
}

ISR(INT1_vect)
{
   cli();
   EIFR |=0x02;
   in = INT1_SW;
   sei();
}

int main()
{
   cli();

   DDRD = 0xF3;
   DDRB = 0xFF;
   DDRC = 0xFF;
   PORTC = 0xFF;

   disp_seg(cnt);

   EICRA = 0x03;
   EIMSK = 0x03;
   EIFR = 0x03;

   sei();


   while(1)
   {
      if(in == INT1_SW)
      {
         in = 0;
         for(int i=0;i<10;i++)
         {   
            if(in == INT0_SW|in == INT1_SW)
            {
               disp_seg(cnt);
               break;
            }
            LED_ALL_ON_OFF();
         }
         in = 0;
      }
   }
}
