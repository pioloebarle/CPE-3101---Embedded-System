#include<xc.h> // include file for the XC8 compiler
#pragma config FOSC = XT
#pragma config WDTE = OFF
#pragma config PWRTE = ON
#pragma config BOREN = ON
#pragma config LVP = OFF
#pragma config CPD = OFF
#pragma config WRT = OFF
#pragma config CP = OFF

unsigned int period_flag = 0;
float period = 0.001;
unsigned int duty_flag = 0;
float percent = 0;
unsigned int result = 0;
float time = 0;

void interrupt ISR()
{

	GIE = 0;
	if(ADIF){
		ADIF = 0;
				
	}

	GIE = 1;
}

void delay(int MUL) {
    int i, j;
    for (i = MUL; i != 0; i--) {
        for (j = 0; j < 256; j++) {} 
    }
}

int readADC(void){
	int temp = 0;
	delay(100);
	GO = 1;
	while(GO_DONE){}
	
	temp = ADRESH;
	temp = temp << 8;
	temp = temp | ADRESL;
	return temp;
}

void main()
{	
	int d_value = 0;
	TRISB = 0x00;
	//TRISA = 0xFF;
	PORTB = 0x00;
	ADCON1 = 0x80;		//result: right justified, clock: FOSC/8
						//all ports in PORTA are analog
						//VREF+ = VDD, VREF- = VSS
						
	ADCON0 = 0x41;		// clock: FOSC/8, analog channel:AN0(RA0)
						//A/D Conversion: STOP, A/D Module: ON

	while(1)
	{
		d_value = readADC();
		if(d_value >= 0 && d_value <= 169){
			PORTB = 0x00;
		}
		else if(d_value >= 170 && d_value <= 340){
			PORTB = 0x01;
		}
		else if(d_value >= 341 && d_value <= 511){
			PORTB = 0x03;
		}
		else if(d_value >= 512 && d_value <= 682){
			PORTB = 0x07;
		}
		else if(d_value >= 683 && d_value <= 853){
			PORTB = 0x0F;
		}
		else if(d_value >= 854 && d_value <= 1024){
			PORTB = 0x1F;
		}
		
	}	
		
}