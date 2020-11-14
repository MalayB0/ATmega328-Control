void led_on_off_alt(int n, int delay_time)
{
	DDRD = 0x30;
	DDRC = 0x3F;

	int i = 0;
	for(i = 0; i < n; i++)
	{
		PORTD = (0xF0 & 0x30) | (PIND & 0xCF);
		PORTC = (0xF0 & 0x3F) | (PINC & 0xC0);
		_delay_ms(delay_time);

		PORTD = (0x0F & 0x30) | (PIND & 0xCF);
		PORTC = (0x0F & 0x3F) | (PINC & 0xC0);
		_delay_ms(delay_time);
	}

	// clear
	PORTD = (0x00 & 0x30);
	PORTC = (0x00 & 0x3F);
}
