#include<xc.h> // include file for the XC8 compiler
#pragma config FOSC = XT
#pragma config WDTE = OFF
#pragma config PWRTE = ON
#pragma config BOREN = ON
#pragma config LVP = OFF
#pragma config CPD = OFF
#pragma config WRT = OFF
#pragma config CP = OFF

bit TMR0IF_flag = 0;
bit INTF_flag = 0;
int load = 0x00;

void interrupt ISR()
{

	GIE = 0;
	if(TMR2IF){
		TMR2IF = 0;
		RA0 = RA0^1; 
		
	}

	GIE = 1;
}

void delay(int count)
{
	int OF_count = 0;
    
    while (OF_count < count)
    {
        if (TMR0IF_flag == 1)
        {
            OF_count++;
            TMR0IF_flag = 0;
        }
    }
}

void main()
{	
	ADCON1 = 0x06; // set all pins in PORTA as digital I/O
	TRISA = 0x00; // sets all of PORTA to output
	RA0 = 0; // initialize RA0 to 0 (LED off)
	T2CON = 0x01; // 1:8 prescaler, internal clock, Timer1 off
	TMR2IE = 1; // enable Timer1 overflow interrupt (PIE1 reg)
	TMR2IF= 0; // reset interrupt flag (PIR1 reg)
	PEIE = 1; // enable all peripheral interrupt (INTCON reg)
	GIE = 1; // enable all unmasked interrupts (INTCON reg)
	PR2 = 0x7D;
	TMR2ON = 1; // Turns on Timer1 (T1CON reg)

	while(1)
	{
		
		
	}
}
