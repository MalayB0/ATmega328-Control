int LED_ANTERNATING_ON_OFF(int a)
{
	for(int i=0 ; i<a ; i++){
	PORTC = 0xF0;
	PORTD = (PORTD & 0x0C)|0xFF;
	_delay_ms(400);
	PORTC = 0x0F;
	PORTD = (PORTD & 0x0C)|0x00;
	_delay_ms(400);
	}
}
