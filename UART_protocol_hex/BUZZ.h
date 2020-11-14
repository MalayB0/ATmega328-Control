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
