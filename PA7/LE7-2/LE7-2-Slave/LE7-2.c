#include<xc.h> // include file for the XC8 compiler
#pragma config FOSC = XT
#pragma config WDTE = OFF
#pragma config PWRTE = ON
#pragma config BOREN = ON
#pragma config LVP = OFF
#pragma config CPD = OFF
#pragma config WRT = OFF
#pragma config CP = OFF

void init_I2C_Slave(unsigned char slave_add)
{
	 TRISC3 = 1; 			// set RC3 (SCL) to input
	 TRISC4 = 1; 			// set RC4 (SDA) to input
	 SSPCON = 0x36; 		// SSP enabled, SCK release clock
	 						// I2C slave mode 7-bot address
	 SSPCON2 = 0x01; 		// start condition idle, stop condition idle
	 						// receive idle, clock stretching
	 SSPSTAT = 0x80; 		// slew rate control disabled
	 SSPADD = slave_add; 	// 7-bit slave address
	 SSPIE = 1; 			// enable SSP interrupt
	 SSPIF = 0; 			// clear interrupt flag
	 PEIE = 1; 				// enable peripheral interrupt
	 GIE = 1; 				// enable unmasked interrupt
} 
void interrupt ISR()
{
	unsigned char temp;
	CKP = 0;						// holds clock low (SSPCON REG)
	if(WCOL || SSPOV)				// check if overflow or data collision (SSPCON REG)
	{
		temp = SSPBUF;				// read SSPBUF to clear buffer
		WCOL = 0;					// clear data collision flag
		SSPOV = 0;					// clear overflow flag
		CKP = 1;					// release clock (SSPCON REG)				
	}
	
	if(!SSPSTATbits.D_nA && !SSPSTATbits.R_nW) 			// write to slave
	{
	 	temp = SSPBUF;
	 	while(!BF); 				// wait until receive is complete (SSPSTAT reg)
	 	
	 	/* read data from SSPBUF */
	 	PORTB = SSPBUF;
	 	CKP = 1; 					// release clock (SSPCON reg)
	}
	else if(!SSPSTATbits.D_nA && SSPSTATbits.R_nW) 		// read from slave
	{
	 	temp = SSPBUF;
	 	BF = 0; 					// clear buffer status bit (SSPSTAT reg)
	 	
	 	/* send data by writing to SSPBUF */
	 	SSPBUF = PORTD; 
	 	CKP = 1; 					// release clock (SSPCON reg)
	 	while(BF); 					// wait until transmit is complete (SSPSTAT reg)
	}
	
	SSPIF = 0; // clear interrupt flag 

}
void delay(int MUL) {
    int i, j;
    for (i = MUL; i != 0; i--) {
        for (j = 0; j < 256; j++) {} 
    }
}

/* Slave Device */
void main()
{	
	TRISB = 0x00;					// set all bits in PORTB to output
	PORTB = 0x00;					// all LEDs in PORTB are off
	TRISD = 0xFF;					// set all bits in PORTD to input
	
	init_I2C_Slave(0x10);			// initialize I2C as slave with address 0x01
	
	while(1)
	{	
	}
}