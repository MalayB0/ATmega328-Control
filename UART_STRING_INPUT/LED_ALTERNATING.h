void LED_ALTERNATING(unsigned char a)
{
	for(int i=0;i<a;i++)
	{
		PORTB = (PORTB&0x02)|0x28;
		PORTC = (PORTC&0x01)|0x14;
		_delay_ms(300);
		PORTB = (PORTB&0x02)|0x14;
		PORTC = (PORTC&0x01)|0x0A;
		_delay_ms(300);
	}

}
