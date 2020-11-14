#include <avr/io.h>
#define F_CPU 16000000UL
#include <util/delay.h>
#include <avr/interrupt.h>
//unsigned char TovVal = 0;

void LED_BLINK(void)
{
   PORTD = 0x00;
   _delay_ms(300);
   PORTD = 0xFF;
   _delay_ms(300);
}

ISR(TIMER1_OVF_vect)
{
   TCNT1 = 0xFFFF - 0x3D09; // 초기값을 0 이 아닌 65536 - 49910 로 설정하여 1초가능하게 셋팅
   LED_BLINK();
   sei();
}

int main()
{
   cli();

   DDRD = 0xFF;
   PORTD = 0xFF;

   TCCR1A = 0x0000; // NORMAL MODE, TCCRnX 가 모드설정 p132
   
   TCCR1B |= (1 << CS10); // p133
   TCCR1B |= (1 << CS12); // prescaler 1024 로 설정

   TCNT1 = 0xFFFF - 0x3D09; // 65536 - 49910(FFFF-(원하는 시간*16,000,000/prescale)) //To generate TOV at every 1s

   TIMSK1 |= (1 << TOIE1); // Timer1 overflow interrupt enable
   TIFR1 |= (1 << TOV1);   // interrupt 가 가능하게 flag 를 1값으로 세움

   sei();

   while(1){}
}
