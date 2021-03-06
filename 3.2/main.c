/*******************************************
reverse led use FALL interrupt (外部中断翻转led)
*******************************************/

#include <msp430f6638.h>

typedef volatile unsigned int u16;

u16 counter1 = 0,counter2 = 0;

void main(void)
{
	WDTCTL = WDTPW + WDTHOLD;   // Stop watchdog timer
	P4DIR |= BIT4+BIT5+BIT6;    // Set P4.4 P4.5 P4.6 for output   0111 0000
	P4DIR &= ~(BIT2+BIT3);      // Set P4.2 P4.3 for input
	P4IE |= BIT2+BIT3;          // Allow P4.2 P4.3 interrupt
	P4IES |= BIT2+BIT3;         // P4.2 P4.3选择下降沿触发
	P4IFG &= ~(BIT2 + BIT3);    // Clear interrupt flag
	_enable_interrupt();        // Enable total interrupt
	P4OUT |= BIT4 + BIT5;
	while(1);
}

// 中断服务程序
#pragma vector = PORT4_VECTOR
__interrupt void KEYCONTROL(void)
{
		counter1++;
		if(counter1 == 5)
		{
			P4OUT ^= BIT4;           // Reverse P4.4
			counter1 = 0;
		}
		P4IFG &= ~BIT2;          // Clear interrupt flag
}
