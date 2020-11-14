#include <math.h>
#define ON	1
#define OFF 0

int on_off = 0;

void play(unsigned int y){
	DDRB |= 0x02;
	TCCR1A |=  0X40; //(1 << COM1A0); // 0x40
	TCCR1B |=  0x1A; //(1 << WGM13)| (1 << WGM12)| (1 << CS11); // 0x1A
	ICR1 = y;
}

void stop(void){
	TCCR1A = 0x00;
	TCCR1B = 0x00;
	TCNT1 = 0;
	ICR1 = 0;

	DDRB = ~0x02;
}

void buzzer_on_off(unsigned char note){
	
	on_off = note;
	
	while(on_off == ON){

		play(2273);
		}
	if(on_off == OFF){
		Printf("stop ena");
		stop();
	}else{
		Printf("wrong note\n");
	}
}
