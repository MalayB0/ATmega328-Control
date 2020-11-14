#include <stdio.h>
void Printf(char *fmt, ...)
{
	va_list 	arg_ptr;
	uint8_t 	i,len;
	char 	sText[128];
		
	/* text buffer */
	for (i = 0; i < 128; i++)
		sText[i] = 0;

	va_start(arg_ptr, fmt);
	vsprintf(sText, fmt, arg_ptr);
	va_end(arg_ptr);

	len = strlen(sText);
	for (i = 0; i < len; i++){
		tx_char(sText[i]);
	}
}
void tx_char(unsigned char txChar){
	while(!(UCSR0A & (1 << UDRE0)));
	UDR0 = txChar;
}
void tx_str(unsigned char *txStr, int len)
{
	int i;
	for(i = 0; i < len; i++)
	{
		tx_char(txStr[i]);
	}
}
