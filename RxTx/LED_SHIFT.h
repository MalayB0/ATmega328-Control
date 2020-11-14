int LED_SHIFT(int a)
{
	PORTC = 0xFF;
	PORTD = (PORTD & 0xC0)|0xFF;
	for(int i=0; i<a; i++){
		for(int i=0; i<6; i++){
		PORTC = ~(0x01 << i); // PORTC 0~5 = LED 0~5, PORTD 0,1 = LED 6,7
		_delay_ms(400);
		PORTC = 0xFF;
		}
		for(int i=0; i<1; i++){
		PORTD = (PORTD & 0x0C)|~(0x10<<i);
		_delay_ms(400);
		PORTD = (PORTD & 0x0C)|0xFF;
		}
		for(int i=0; i<2; i++){
		PORTD = (PORTD & 0x0C)|~(0x20>>i);
		_delay_ms(400);
		PORTD = (PORTD & 0x0C)|0xFF;
		}
		for(int i=0; i<6; i++){
		PORTC = ~(0x20 >> i);
		_delay_ms(400);
		}
	}
}	
