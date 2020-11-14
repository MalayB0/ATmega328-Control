void BUZZ()
{
	DDRB |= 0x02;
	TCCR1A |= 0x40;
	TCCR1B |= 0x1A;
	ICR1 = FREQ(1000);
}

void stop(void)
{
	TCCR1A |= 0x00;
	TCCR1B |= 0x00;
	TCNT1 = 0;
	ICR1 = 0;
	DDRB = ~0x02;
}
