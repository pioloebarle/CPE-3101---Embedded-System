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
	PR2 = 0x7C;        
	// set value for PR2 
	CCPR1L = 0x57;     
	CCP1CON = 0x2C;    
	TRISC = 0x00;      
	RC2 = 0;           
	T2CON = 0x06; 

	while(1)
	{
		
		
	}
}