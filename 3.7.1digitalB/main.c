#include <msp430f6638.h>

#define d 0x01
#define c 0x20
#define b 0x40
#define a 0x80
#define dp 0x10
#define g 0x04
#define f 0x08
#define e 0x02

volatile unsigned int time,i,j,k,index,average;
volatile float AVERAGE;
unsigned int long result[9];

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

void Backlight_lcd(unsigned int x)
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

void ADC12Set()
{
	P6SEL |= BIT6;
	ADC12CTL0 = ADC12MSC + ADC12ON + ADC12SHT0_15;
	ADC12CTL1 = ADC12SHP + ADC12CONSEQ_2 + ADC12SSEL_1;
	ADC12MCTL0 = ADC12INCH_6;
	ADC12IE |= ADC12IE0;
	ADC12CTL0 |= ADC12ENC;
	ADC12CTL0 |= ADC12SC;
}

void ADC12DisposalFunction()
{
	result[index++] = ADC12MEM0;
	if(index == 8)
	{
		average = 0;
		for(k=0;k<8;k++)
		{
			average += result[k];
			result[k] = 0;
		}
		index = 0;
		average >>= 3;
	}
}

void DataDisposalB()
{
	LCDMEM[2] = number[average / 1000];
	LCDMEM[3] = number[(average / 100) % 10];
	LCDMEM[4] = number[(average / 10) % 10];
	LCDMEM[5] = number[average % 10];
}

void DataDisposalH()
{
	LCDMEM[3] = number[(average / 16) / 16];
	LCDMEM[4] = number[(average / 16) % 16];
	LCDMEM[5] = number[average % 16];
}

void DataDisposalA()
{
	average *= 0.806;
	LCDMEM[2] = number[average / 1000];
	LCDMEM[3] = number[(average / 100) % 10] + dp;
	LCDMEM[4] = number[(average / 10) % 10];
	LCDMEM[5] = number[average % 10];
}

void DisposalSelect(unsigned int sign)
{
	if(sign == 1)
	{
		DataDisposalB();
	}
	if(sign == 2)
	{
		DataDisposalH();
	}
	if(sign == 3)
	{
		DataDisposalA();
	}
}

void main(void)
{
	WDTCTL = WDTPW + WDTHOLD;
	Init_lcd();
	Start_lcd(1);
	Clear_lcd();
	Init_TS3A5017();
	Backlight_lcd(1);
	ADC12Set();
	_BIS_SR(GIE);
}

#pragma vector = ADC12_VECTOR
__interrupt void VoltageDisplay()
{
	ADC12DisposalFunction();
	DisposalSelect(3);
}
