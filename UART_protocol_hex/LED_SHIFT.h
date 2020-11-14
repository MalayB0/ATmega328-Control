void LED_SHIFT(unsigned char a)
{
	PORTB = (PINB&0x02)|0xFD;
	PORTC = (PINC&0x01)|0xFE;
	for(int j = 0;j<a;j++)
	{
		for(int i=0;i<5;i++)
		{
			_delay_ms(200);
			PORTB = (PINB&0x02)|((~(0x04<<i))&0x3C);
		}
		for(int i=0;i<4;i++)
		{
			PORTC = (PINC&0x01)|((~(0x02<<i))&0x1E);
			_delay_ms(200);
		}
		for(int i=0;i<5;i++)
		{
			_delay_ms(200);
			PORTC = (PINC&0x01)|((~(0x10>>i))&0x1E);
		}
		for(int i=0;i<4;i++)
		{
			PORTB = (PINB&0x02)|((~(0x20>>i))&0x3C);
			_delay_ms(200);
		}
	}
}
