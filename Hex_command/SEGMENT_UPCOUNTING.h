unsigned char seg[] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x27, 0x7F, 0x67};
int SEGMENT_UPCOUNTING(int a)
{
	for(int i=0; i<a; i++){
	int i = 0;
	PORTD = (PORTD&0xC0)|(seg[i++%10]<<6);
	PORTB = (PORTB & 0xCC)|seg[i++%10];
	_delay_ms(400);
	}
}

