int LED_ALL_ON_OFF(int a)
{
	for(int i=0 ; i<a; i++){
	PORTC = 0x00;
	PORTD = (PORTD & 0x0C)|0x00;
	_delay_ms(400);
	PORTC = 0xFF;
	PORTD = (PORTD & 0x0C)|0xFF;
	_delay_ms(400);
	}
}
