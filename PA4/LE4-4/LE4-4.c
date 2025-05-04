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
	if(CCP1IF){
		CCP1IF = 0;
		RA0 = RA0 ^ 1;
		
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
	TRISA = 0x00;
	PORTA = 0x00;
	ADCON1 = 0x06;
	T1CON = 0x20;     // 1:8 prescaler, Timer1 off 
	CCP1CON = 0x0A;    // capture mode: every rising edge 
	CCP1IE = 1;       // enable TMR1/CCP1 match interrupt (PIE1 reg) 
	CCP1IF = 0;       // reset interrupt flag (PIR1 reg) 
	CCPR1 = 0x4E2;
	PEIE = 1;         // enable all peripheral interrupt (INTCON reg) 
	GIE = 1;          // enable all unmasked interrupts (INTCON reg) 
	TMR1ON = 1;       // Turns on Timer1 (T1CON reg) 

	while(1)
	{
		
		
	}
}