void led_all_on_off(int n, unsigned int delay_time)
{
	DDRD = 0x30;
	DDRC = 0x3F;

	for(int i = 0; i < n; i++)
	{
		PORTD = 0x30 | (PIND & 0xCF);
		PORTC = 0x3F | (PINC & 0xC0);
		_delay_ms(delay_time);

		PORTD = 0x00 | (PIND & 0xCF);
		PORTC = 0x00 | (PINC & 0xC0);
		_delay_ms(delay_time);
	}
}
