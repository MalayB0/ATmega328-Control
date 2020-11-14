void LED_ALL_ON_OFF(unsigned char a)
{
	for(int i=0;i<a;i++)
	{
		PORTB = (PINB&0x02);
		PORTC = (PINC&0x01);
		_delay_ms(300);
		PORTB = (PINB&0x02)|0x3C;
		PORTC = (PINC&0x01)|0x1E;
		_delay_ms(300);
	}
}
