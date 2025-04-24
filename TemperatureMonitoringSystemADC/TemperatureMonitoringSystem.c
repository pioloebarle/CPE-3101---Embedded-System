#include<xc.h> // include file for the XC8 compiler
#pragma config FOSC = XT
#pragma config WDTE = OFF
#pragma config PWRTE = ON
#pragma config BOREN = ON
#pragma config LVP = OFF
#pragma config CPD = OFF
#pragma config WRT = OFF
#pragma config CP = OFF

bit ADIF_flag = 0;
unsigned int adc_result = 0;
unsigned int whole, decimal, decimal1;
unsigned int tens, ones;
unsigned char dispTemp[6];
unsigned int result = 0;

void delay(int MUL) {
    int i, j;
    for (i = MUL; i != 0; i--) {
        for (j = 0; j < 256; j++) {} 
    }
}

void interrupt ISR()
{
	GIE = 0;
	if(ADIF){
		ADIF = 0;
		ADIF_flag = 1;
		
		adc_result = (ADRESH << 8) | ADRESL;
		
		float analogVoltage = (adc_result * 5.0) / 1023;
		result = (unsigned int)(analogVoltage * 100);
		
		tens = result / 10;
		ones = result % 10;
//		whole = result / 100;
//		decimal = (result % 100) / 10;
//		decimal1 = result % 10;
		
	}
	
	GIE = 1;
}

void delayLCD(int MUL) {
    int i, j;
    for (i = MUL; i != 0; i--) {
        for (j = 0; j < 256; j++) {} 
    }
}

void instCtrl (unsigned char INST)
{
	PORTB = INST; // load instruction to PORTB
	RC0 = 0; // set RS to 0 (instruction reg)
	RC1 = 0; // set RW to 0 (write)
	RC2 = 1; // set E to 1
	delayLCD(1); // 1 ms delay (estimate)
	RC2 = 0; // set E to 0 (strobe)
}

void dataCtrl (unsigned char DATA)
{
	PORTB = DATA; 		// load data to PORTB
	RC0 = 1; 			// set RS to 1 (data reg)
	RC1 = 0; 			// set RW to 0 (write)
	RC2 = 1; 			// set E to 1
	delayLCD(1); 			// 1 ms delay (estimate)
	RC2 = 0; 			// set E to 0 (strobe)
}
//LCD Display Lines
//LINE 1 (80H - 93H)
//LINE 2 (C0H - D3H)
//LINE 3 (94H - A7H)
//LINE 4 (D4H - E7H)
void initLCD() {
    delayLCD(15);
    instCtrl(0x38); // 8-bit mode, 2-line display
    instCtrl(0x0C); // Display ON, Cursor OFF
    instCtrl(0x06); // Entry mode
    instCtrl(0x01); // Clear display
    delayLCD(2);
}

void display(const char *input) {
    int i;
    for (i = 0; input[i] != '\0'; i++) {
        dataCtrl(input[i]);
    }
}

void main()
{	
	TRISA = 0xFF;
	TRISB = 0x00;
	TRISC = 0x00;
	
	PEIE = 1;
	GIE = 1;
	ADCON1 = 0x80;
	ADCON0 = 0x41;
	ADIE = 1;
	ADIF = 0;
	delay(10);
	GO = 1;
	initLCD();

	while(1)
	{
		if(ADIF_flag){
			
			instCtrl(0x80);
			display("Temp: ");
			dataCtrl(tens + '0');
			//dataCtrl('.');
			dataCtrl(ones + '0');
			display(".0 C");
			//dataCtrl(decimal1 + '0');
			//dataCtrl('V');
			
			ADIF_flag = 0;
			delay(10);
			GO = 1;
			
			if((tens == 4) && (ones == 0)){
				instCtrl(0xC0);
				display("HIGH TEMP!");
			}
			else{
				instCtrl(0xC0);
				display("           ");
			}
		}
		
	}	
		
}