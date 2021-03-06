#include <msp430f6638.h>


#define d 0x01
#define c 0x20
#define b 0x40
#define a 0x80
#define dp 0x10
#define g 0x04
#define f 0x08
#define e 0x02

volatile unsigned int time,i,j;

const char number[] =
{
   a+b+c+d+e+f,                         // Displays "0"
   b+c,                                 // Displays "1"
   a+b+d+e+g,                           // Displays "2"
   a+b+c+d+g,                           // Displays "3"
   b+c+f+g,                             // Displays "4"
   a+c+d+f+g,                           // Displays "5"
   a+c+d+e+f+g,                         // Displays "6"
   a+b+c,                               // Displays "7"
   a+b+c+d+e+f+g,                       // Displays "8"
   a+b+c+d+f+g,                         // Displays "9"
};

void delay(unsigned int duration)
{
	for(time=0 ; time<duration*5000 ; time++);
}

void Init_lcd(void)
{
	LCDBCTL0 = LCDDIV0 + LCDPRE_1 +LCDSSEL + LCDMX0 + LCDMX1 + LCDSON;
	LCDBPCTL0 = LCDS0 + LCDS1 + LCDS2 + LCDS3 + LCDS4 + LCDS5 + LCDS6+ LCDS7 + LCDS8+ LCDS9 + LCDS10 + LCDS11 ;
	P5SEL |= BIT3 + BIT4 + BIT5;
}

void Start_lcd(unsigned char x)
{
	if(x == 1)
	{
		LCDBCTL0 |= LCDON;
	}
	else if(x == 0)
	{
		LCDBCTL0 &= ~LCDON;
	}
}

void Clear_lcd(void)
{
	unsigned int i;
	for(i=0 ; i<7 ; i++)
	{
		LCDMEM[i] = 0;
	}
}

void Init_TS3A5017(void)
{
	P1DIR |= BIT6 + BIT7;    //P1.7 : IN1 ; P1.6 : IN2   set as output
	P1OUT &= ~BIT7;          //IN1 = 0
	P1OUT |= BIT6;           //IN2 = 1
}

void backlight_lcd(unsigned int x)
{
	if(x == 1)
	{
		P8DIR |= BIT0;
		P8OUT |= BIT0;
	}
	else if (x == 0)
	{
		P8DIR |= BIT0;
		P8OUT &= ~BIT0;
	}
}

void Init_TAO_onesecond(void)
{
	TA0CTL |= TASSEL_1 + MC_1 + TACLR;                //ACLK  Add model
	TA0CCTL0 |= CCIE;                                 //Enable interrupt
	TA0CCR0 = 32768;
}


void main(void)
{
	WDTCTL = WDTPW + WDTHOLD;
	Init_lcd();
	Start_lcd(1);
	Clear_lcd();
	Init_TS3A5017();
	backlight_lcd(1);
	_BIS_SR(GIE);
	i = 0;
	Init_TAO_onesecond();
	while(1);
}

// �жϷ������
#pragma vector = TIMER0_A0_VECTOR
__interrupt void circulate(void)
{
	for(j=0 ; j < 6 ; j++)
	{
		LCDMEM[j] = number[(i+j)%6+1];
	}
	i++;
	if(i == 6)
	{
		i = 0;
	}
}
