#define ON   1
#define OFF  0
int on_off = 0;

int BUZZER_ON_OFF(int a)
{
   if(a>0){
   DDRB = 0x02
	   while(1){
	      if(on_off == OFF){
	         play(1000);
	      }
	      else{
	         stop();
	      }
	   }
	}
}

void play(unsigned int y){
   TCCR1A |=  0X40; //(1 << COM1A0); // 0x40
   TCCR1B |=  0x1A; //(1 << WGM13)| (1 << WGM12)| (1 << CS11); // 0x1A
   ICR1 = y;
}

void stop(void){
   TCCR1A = 0x00;
   TCCR1B = 0x00;
   TCNT1 = 0;
   ICR1 = 0;

   DDRB = ~0x02;
}
