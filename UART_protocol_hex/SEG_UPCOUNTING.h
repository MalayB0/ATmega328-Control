unsigned char seg[] = {0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x27,0x7F,0x6F};

void SEG_UPCOUNTING(int a)
{
	for(int j = 0; j<a; j++)
	{
		for(int i = 0; i<10;i++)
		{
			PORTB = (PINB&0x02)|((seg[i%10]<<2)&0x3C);
			PORTC = (PINC&0x01)|((seg[i%10]>>3)&0x1E);
			_delay_ms(200);
		}
	}
}
