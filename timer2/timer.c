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
   TCNT1 = 0xFFFF - 0x3D09; // �ʱⰪ�� 0 �� �ƴ� 65536 - 49910 �� �����Ͽ� 1�ʰ����ϰ� ����
   LED_BLINK();
   sei();
}

int main()
{
   cli();

   DDRD = 0xFF;
   PORTD = 0xFF;

   TCCR1A = 0x0000; // NORMAL MODE, TCCRnX �� ��弳�� p132
   
   TCCR1B |= (1 << CS10); // p133
   TCCR1B |= (1 << CS12); // prescaler 1024 �� ����

   TCNT1 = 0xFFFF - 0x3D09; // 65536 - 49910(FFFF-(���ϴ� �ð�*16,000,000/prescale)) //To generate TOV at every 1s

   TIMSK1 |= (1 << TOIE1); // Timer1 overflow interrupt enable
   TIFR1 |= (1 << TOV1);   // interrupt �� �����ϰ� flag �� 1������ ����

   sei();

   while(1){}
}
