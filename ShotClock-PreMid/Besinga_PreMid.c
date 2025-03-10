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

const unsigned char Count[] = "Count";
unsigned char MSBcounter = 0x02;
unsigned char LSBcounter = 0x04; 
unsigned char load = ' ';
bit state = 0;
bit direction = 0;
bit count = 0;




void interrupt ISR()
{
	GIE = 0;
	int DATA;
	if(INTF == 1){
		INTF = 0;
		INTF_flag = 1;
		DATA = PORTD & 0x0F;
		switch(DATA){
			case 0x0C:
				load = '*';
				direction = 1;
				break;
			case 0x0D:
				load = '0';
				state ^= 1;
				break;
			case 0x0E:
				load = '#';
				direction = 0;
				break;
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
	RB5 = 0; // set RS to 0 (instruction reg)
	RB6 = 0; // set RW to 0 (write)
	RB7 = 1; // set E to 1
	delayLCD(1); // 1 ms delay (estimate)
	RB7 = 0; // set E to 0 (strobe)
}

void dataCtrl (unsigned char DATA)
{
	PORTC = DATA; 		// load data to PORTB
	RB5 = 1; 			// set RS to 1 (data reg)
	RB6 = 0; 			// set RW to 0 (write)
	RB7 = 1; 			// set E to 1
	delayLCD(1); 			// 1 ms delay (estimate)
	RB7 = 0; 			// set E to 0 (strobe)
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

void numberDisplay(unsigned char MSB, unsigned char LSB){
	instCtrl(0x9D);
	dataCtrl(MSB + 0x30);
	dataCtrl(LSB + 0x30);
}

void countUp(){
	if((MSBcounter == 0x02) & (LSBcounter == 0x04)){
		MSBcounter = 0x00;
		LSBcounter = 0x00;
	}
	else if (LSBcounter == 0x09){
		LSBcounter = 0x00;
		MSBcounter++;
	}
	else{
		LSBcounter++;
	}
	delay(31);
	numberDisplay(MSBcounter,LSBcounter);		
}

void countDown(){
	if((MSBcounter == 0x00) & (LSBcounter == 0x00)){
		MSBcounter = 0x02;
		LSBcounter = 0x04;
	}
	else if (LSBcounter == 0x00){
		LSBcounter = 0x09;
		MSBcounter--;
	}
	else{
		LSBcounter--;
	}
	delay(31);
	numberDisplay(MSBcounter,LSBcounter);
		
}

void displayCount(){
	instCtrl(0xC7);
	display(Count);
}





void main(){
	int DATA;
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
		if(INTF_flag){
			switch(load){
				case '*':
					if((state == 0) && (direction == 1)){
						countUp();
						direction = 0;
					}
					else if ((state == 1) && (direction == 1)){
						countDown();
					}
					break;
				case '0':
					if(state == 1){
						countDown();
					}
					else if (state == 0){
						numberDisplay(MSBcounter,LSBcounter);
					}
					break;
				case '#':
					if((state == 0) && (direction == 0)){
						countDown();
						direction = 1;
					}
					else if ((state == 1) && (direction == 0)){
						countDown();
					}
					default: break;
			}
			

		}
	

		

				
		

		

			
	}

}
