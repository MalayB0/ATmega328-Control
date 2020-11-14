unsigned char array[] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7C, 0x07, 0x7F, 0X67};

void seg_upcounting(int n){
	//PC(0:3) = SEG(0:3) ,PB(2:5) = SEG(4:7)
	DDRB = 0x3C;
	DDRC = 0x0F;

	int i = 0;
	for(i = 0; i < n+1; i++){
		PORTC = (array[i] & 0x0F) | (PINC & 0xF0);
		PORTB = ((array[i] & 0xF0) >> 2 ) | (PINB & 0x03);
		_delay_ms(300);		
	}
}

