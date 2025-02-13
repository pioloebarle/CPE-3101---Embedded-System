#include<xc.h> // include file for the XC8 compiler
#pragma config FOSC = XT
#pragma config WDTE = OFF
#pragma config PWRTE = ON
#pragma config BOREN = ON
#pragma config LVP = OFF
#pragma config CPD = OFF
#pragma config WRT = OFF
#pragma config CP = OFF



void main(){


	

	TRISA = 0x00;
	TRISD = 0xFF;
	
 	int DATA;
	
	while(1){

		
		
		if(RD4 == 1){
			DATA = PORTD & 0x0F;
			while(RD4){
				switch(DATA){
					case 0x00: PORTA = 0x01; break;
					case 0x01: PORTA = 0x02; break;
					case 0x02: PORTA = 0x03; break;
					case 0x04: PORTA = 0x04; break;
					case 0x05: PORTA = 0x05; break;
					case 0x06: PORTA = 0x06; break;
					case 0x08: PORTA = 0x07; break;
					case 0x09: PORTA = 0x08; break;
					case 0x0A: PORTA = 0x09; break;
					case 0x0C: PORTA = 0x0F; break;
					case 0x0D: PORTA = 0x00; break;
					case 0x0E: PORTA = 0x0F; break;
				
				}
			}
			
		}

	}
	return;
}