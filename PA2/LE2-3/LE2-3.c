#include<xc.h> // include file for the XC8 compiler
#pragma config FOSC = XT
#pragma config WDTE = OFF
#pragma config PWRTE = ON
#pragma config BOREN = ON
#pragma config LVP = OFF
#pragma config CPD = OFF
#pragma config WRT = OFF
#pragma config CP = OFF


void delay (int MUL)
{
	int i,j;
	for (i=MUL; i != 0; i--){
		for (j=0; j < 256; j++){} // loop until j = 0
	} // loop until i = 0
	
}

void instCtrl (unsigned char INST)
{
	PORTB = INST; // load instruction to PORTB
	RC0 = 0; // set RS to 0 (instruction reg)
	RC1 = 0; // set RW to 0 (write)
	RC2 = 1; // set E to 1
	delay(1); // 1 ms delay (estimate)
	RC2 = 0; // set E to 0 (strobe)
}

void dataCtrl (unsigned char DATA)
{
	PORTB = DATA; 		// load data to PORTB
	RC0 = 1; 			// set RS to 1 (data reg)
	RC1 = 0; 			// set RW to 0 (write)
	RC2 = 1; 			// set E to 1
	delay(1); 			// 1 ms delay (estimate)
	RC2 = 0; 			// set E to 0 (strobe)
}

void initLCD()
{
	delay(1); // LCD startup about approx 15ms
	instCtrl(0x38); // function set: 8-bit; dual-line
	instCtrl(0x08); // display off
	instCtrl(0x01); // display clear
	instCtrl(0x06); // entry mode: increment; shift off
	instCtrl(0x0E); // display on; cursor on; blink off	
}




void main(){

	TRISB = 0x00;
	TRISC = 0x00;
	TRISD = 0x1F;
	unsigned LCD_pos = 0x80;
	initLCD();

	
	
	unsigned char DATA;

	while(1){
		instCtrl(LCD_pos);

		if(RD4){
			DATA = PORTD & 0x0F;
			while(RD4){}
			switch(DATA){
				case 0x00: dataCtrl('1'); break;
				case 0x01: dataCtrl('2'); break;
				case 0x02: dataCtrl('3'); break;
				case 0x04: dataCtrl('4'); break;
				case 0x05: dataCtrl('5'); break;
				case 0x06: dataCtrl('6'); break;
				case 0x08: dataCtrl('7'); break;
				case 0x09: dataCtrl('8'); break;
				case 0x0A: dataCtrl('9'); break;
				case 0x0C: dataCtrl('*'); break;
				case 0x0D: dataCtrl('0'); break;
				case 0x0E: dataCtrl('#'); break;
				default: break;
			}

			if(LCD_pos == 0x93){
				LCD_pos = 0xBF;	
			}
			if(LCD_pos == 0xD3){
				LCD_pos = 0x93;	
			}
			if(LCD_pos == 0xA7){
				LCD_pos = 0xD3;	
			}
			if(LCD_pos == 0xE8){
				instCtrl(0x01);
				LCD_pos = 0x7F;
			}			
			LCD_pos++;		
		}


	}

}