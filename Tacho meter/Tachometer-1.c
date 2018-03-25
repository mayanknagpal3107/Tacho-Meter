#include <avr/io.h>
#include <compat/deprecated.h>
#include <util/delay.h>
#include "uart.h"
#include "uart.c"
#include <avr/interrupt.h>
volatile int count=0,g=0,b=0;
volatile char str1[2],str2[2],str3[2];

unsigned int frequency_measure()
{

	int t,i=0;
	//unsigned long z; 
	float x,z;
	
	TCCR1A=0x00;
	TCCR1B= 0x00;  //ICP1 EDGE select and clock set
	_delay_ms(30);

		///first setr TCNT0 to 0 to start on first pulse///


		while(!(TIFR&(1<<ICF1)));   //input capture flag set??
		
		TCNT1=0;
		TIFR=(1<<ICF1);    //always clear flag

		//now capture stART//

		while(!(TIFR&(1<<ICF1)));
		TCCR1B= 0x05;

		t=ICR1+(count*65535);  //STARTING VALUE TO MEASURE

		TIFR=(1<<ICF1);

		//now capture ending value to measure period//


		while(!(TIFR&(1<<ICF1))); //input capture flag set third time

		i=ICR1+(count*65536);
		TIFR=(1<<ICF1); 

		//always clear flag & doing the substraction get the pulse width//

		x=i-t;

		z=((float)x*69.44);//1000000;
		return ((1/z)*1000000);
}

ISR(TIMER1_OVF_vect)
{
	count++;
}

int main()

{
	
	unsigned int r=0;

	uart_init(UART_BAUD_SELECT(115200,F_CPU));	
	while(1)
		{	
			r=(unsigned int)frequency_measure();
			uart_puts(itoa(r,str1,10));
			_delay_ms(100);
			//freq_measure();
			uart_puts("\nhello\r");
	
		}

} 


