#include<xc.h> // include file for the XC8 compiler
#pragma config FOSC = XT
#pragma config WDTE = OFF
#pragma config PWRTE = ON
#pragma config BOREN = ON
#pragma config LVP = OFF
#pragma config CPD = OFF
#pragma config WRT = OFF
#pragma config CP = OFF

void delay(int MUL) {
    int i, j;
    for (i = MUL; i != 0; i--) {
        for (j = 0; j < 256; j++) {} 
    }
}

//void interrupt ISR()
//{
//	GIE = 0;
//	if(RCIF){
//		PORTD = RCREG;
//	}
//
//	GIE = 1;
//
//}



void main()
{	
	SPBRG = 0x19; 		// 9.6K baud rate @ FOSC=4MHz, asynchronous high speed
						// (see formula in Table 10-1)
	SYNC = 0; 			// asynchronous mode (TXSTA reg)
	SPEN = 1; 			// enable serial port (RCSTA reg)
	
	//Transmitter
	TX9 = 0; 			// 8-bit transmission (TXSTA reg)
	BRGH = 1; 			// asynchronous high-speed (TXSTA reg)
	TXEN = 1;			// transmit enable (TXSTA reg)
	RX9 = 0;
	//Receiver
	CREN = 1;
	
	PEIE = 1;
	GIE = 1;
	
	
	TRISC6 = 0;
	TRISC7 = 1;
	
	//PORTS FOR SWITCHES
	TRISB = 0xFF;
	
	
	//PORTS FOR 7SEG
	TRISD = 0x00;
	PORTD = 0x00;
	
	
	
	while (1) {
		
		//LEDs
		unsigned char DATA = PORTB & 0x0F;
		PORTD = (DATA << 4);
		
		
		//7Seg
		if(RB4){
			while(RB4){}
			while(!TXIF);
			TXREG = DATA;	
		}
		
		if(RCIF){
			if(OERR){
				CREN = 0;
				CREN = 1;
			}
		}
		
			//while(!RCIF);
			//unsigned char receiveData = RCREG;
			PORTD = RCREG;
			//RCIE = 1;
		
		
		
 	}       
		
}