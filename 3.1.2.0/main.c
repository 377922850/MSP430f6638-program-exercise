#include <msp430f6638.h>

volatile unsigned int i;

void delay(volatile unsigned int  duration)             //delay function
{
	for(i=0 ; i<5000*duration ; i++);
}

void keycontrol()
{
	if((P4IN & BIT2) == 0)           //confirm keydown:P4.2
	{
	delay(20);                   //delay 20ms : eliminate key shake
	if((P4IN & BIT2) == 0)       //confirm keydown again

		while((P4IN & BIT2) == 0)   //confirm keyup : P4.2
			delay(20);                   //delay 20ms : eliminate key shake
		if ((P4IN & BIT2) != 0)
			{
				P4OUT ^= BIT4;
			}


	}
}

void main(void)
{
	WDTCTL = WDTPW + WDTHOLD;	// Stop watchdog timer
	P4DIR |= BIT4 + BIT5 + BIT6 + BIT7;
	P4DIR &= ~(BIT2 + BIT3);         // Set P4.2 P4.3 for input
	P4OUT = 0;

	while(1)
	{
		keycontrol();
	}
}
