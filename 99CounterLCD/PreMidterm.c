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

#define ONE_SEC 122
const unsigned char Count[] = "Count";
const unsigned char cntUp[] = "Count Up";
const unsigned char cntDown[] = "Count Down";
unsigned char LSBcounter = 0x00; 
unsigned char MSBcounter = 0x00;
unsigned char load = ' ';
bit direction = 0;
int speed = 122;



void interrupt ISR()
{
	GIE = 0;
	int DATA;
	if(INTF == 1){
		INTF = 0;
		INTF_flag = 1;
		DATA = PORTD & 0x0F;
 		switch (DATA){
 			case 0x00: load = '1'; break;
 			case 0x01: load = '2'; break;
 			case 0x02: load = '3'; break;
 			case 0x04: load = '4'; break;
 			case 0x05: load = '5'; break;
 			case 0x06: load = '6'; break;
 			case 0x08: load = '7'; break;
 			case 0x09: load = '8'; break;
 			case 0x0A: load = '9'; break;
 			case 0x0C: load = '*'; break;
 			case 0x0D: load = '0'; break;
 			case 0x0E: load = '#'; break;
 			default: break;
 		}
		
		
	}
	else if(TMR0IF == 1){
		TMR0IF = 0;
		TMR0IF_flag = 1;
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

void delayLCD (int MUL)
{
	int i,j;
	for (i=MUL; i != 0; i--){
		for (j=0; j < 256; j++){} // loop until j = 0
	} // loop until i = 0
	
}

void instCtrl (unsigned char INST)
{
	PORTC = INST; // load instruction to PORTB
	RB1 = 0; // set RS to 0 (instruction reg)
	RB2 = 0; // set RW to 0 (write)
	RB3 = 1; // set E to 1
	delayLCD(1); // 1 ms delay (estimate)
	RB3 = 0; // set E to 0 (strobe)
}

void dataCtrl (unsigned char DATA)
{
	PORTC = DATA; 		// load data to PORTB
	RB1 = 1; 			// set RS to 1 (data reg)
	RB2 = 0; 			// set RW to 0 (write)
	RB3 = 1; 			// set E to 1
	delayLCD(1); 			// 1 ms delay (estimate)
	RB3 = 0; 			// set E to 0 (strobe)
}

void initLCD()
{
	delayLCD(1); // LCD startup about approx 15ms
	instCtrl(0x38); // function set: 8-bit; dual-line
	instCtrl(0x08); // display off
	instCtrl(0x01); // display clear
	instCtrl(0x06); // entry mode: increment; shift off
	instCtrl(0x0C); // display on; cursor on; blink off	
}

void display(const unsigned char* input){
	int i;
	for (i = 0; input[i] != '\0'; i++){
		dataCtrl(input[i]);
	}
}

void dispCntUp(){
	instCtrl(0xC6);
	display(cntUp);
}

void dispCntDown(){
	instCtrl(0xC5);
	display(cntDown);
}

void numberDisplay(unsigned char MSB, unsigned char LSB){
	instCtrl(0x9D);
	dataCtrl(MSB + 0x30);
	dataCtrl(LSB + 0x30);
}

void countUp(){
	if(LSBcounter == 0x09){
		LSBcounter = 0x00;
		if(MSBcounter == 0x09){
			MSBcounter = 0x00;
		}
		else{
			MSBcounter++;
		}
	}
	else{
		LSBcounter ++;
	}
	delay(speed);
	numberDisplay(MSBcounter,LSBcounter);
		
}

void countDown(){
	if(LSBcounter == 0x00){
		LSBcounter = 0x09;
		if(MSBcounter == 0x00){
			MSBcounter = 0x09;
		}
		else{
			MSBcounter--;
		}
	}
	else{
		LSBcounter --;
	}
	delay(speed);
	numberDisplay(MSBcounter,LSBcounter);
		
}

void displayCount(){
	instCtrl(0xC7);
	display(Count);
}





void main(){
	
	OPTION_REG = 0xC4;
	TRISB = 0x01;
	TRISC = 0x00;
	TRISD = 0xFF;

	INTEDG = 1; // interrupt at rising edge
 	INTE = 1; // enable RB0/INT external interrupt
 	INTF = 0; // clears the interrupt flag

	TMR0IF = 0;
    TMR0IE = 1;

	GIE = 1; // enables all unmasked interrupt
	

	initLCD();
	numberDisplay(MSBcounter,LSBcounter);
	displayCount();

	while(1){
		while(INTF_flag == 0){}
		if(INTF_flag == 1){
			switch(load){
				case '1':
					speed = (int)(ONE_SEC * 0.1); 
					break;
				case '2': 
					speed = (int)(ONE_SEC * 0.2);
					break;
				case '3': 
					speed = (int)(ONE_SEC * 0.3);
					break;
				case '4': 
					speed = (int)(ONE_SEC * 0.4);
					break;
				case '5': 
					speed = (int)(ONE_SEC * 0.5);
					break;
				case '6': 
					speed = (int)(ONE_SEC * 0.6);
					break;
				case '7': 
					speed = (int)(ONE_SEC * 0.7);
					break;
				case '8': 
					speed = (int)(ONE_SEC * 0.8);
					break;
				case '9': 
					speed = (int)(ONE_SEC * 0.9);
					break;
				case '0': 
					speed = ONE_SEC;
					break;
				case '*': 
					direction = 0;
					break;
				case '#': 
					direction = 1;
					break;
				default: break;		
			}
		}

		if (direction == 0){
			//dispCntUp();
			countUp();
		}
		else{
			
			//dispCntDown();
			countDown();
		}
		
			
	}

}
