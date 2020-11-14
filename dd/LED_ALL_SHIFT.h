void led_all_shift(int n)
{

	DDRD = 0x30;
	DDRC = 0x3F;

	int cnt = 0;
	int i = 0;
	int j = 0;

	for(cnt = 0; cnt < n; cnt ++)
	{
		for(i = 0; i < 8; i++)
		{
			unsigned char pattern = (0x01 << i);
			PORTD = ((pattern & 0xC0) >> 2) | (PIND & 0xCF);
			PORTC = (pattern & 0x3F);
			_delay_ms(200);
		}
		for(j = 0 ; j < 8; j++ )
		{
			unsigned char pattern = (0x80 >> j);
			PORTD = ((pattern & 0xC0) >> 2) | (PIND & 0xCF);
			PORTC = (pattern & 0x3F);
			_delay_ms(200);
		}
	}

	// clear
	PORTD = 0x00 & 0x30;
	PORTC = 0x00 & 0x3F;
}
