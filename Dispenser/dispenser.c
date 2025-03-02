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

const unsigned char menu1[] = "[1] Coke";
const unsigned char menu2[] = "[2] Sprite";
const unsigned char menu3[] = "[3] Royal";
const unsigned char menu4[] = "[4] Pepsi";
const unsigned char dispense[] = "Dispensing...";
const unsigned char eStop[] = "Emergency Stop...";

void interrupt ISR()
{
	GIE = 0;

	if(INTF == 1){
		INTF = 0;
		INTF_flag = 1;
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
	instCtrl(0x0E); // display on; cursor on; blink off	
}

void display(const unsigned char* input){
	int i;
	for (i = 0; input[i] != '\0'; i++){
		dataCtrl(input[i]);
	}
}

void displayMenu(){
	
	instCtrl(0x80);
	display(menu1);

	instCtrl(0xC0);
	display(menu2);

	instCtrl(0x94);
	display(menu3);

	instCtrl(0xD4);
	display(menu4);	
}

void displayDispense(){
	instCtrl(0x01);
	instCtrl(0xC4);
	display(dispense);
}

void displayEStop(){
	instCtrl(0x01);
	instCtrl(0xC2);
	display(eStop);

}

void main(){
	
	OPTION_REG = 0xC4;
	TRISA = 0x00;
	TRISB = 0x01;
	TRISC = 0x00;
	TRISD = 0xFF;
	TRISE = 0x00;
	

	INTEDG = 1; // interrupt at rising edge
 	INTE = 1; // enable RB0/INT external interrupt
 	INTF = 0; // clears the interrupt flag

	TMR0IF = 0;
    TMR0IE = 1;

	GIE = 1; // enables all unmasked interrupt
	
	ADCON1 = 0x06;

	int DATA;

	unsigned LCD_pos = 0x80;
	initLCD();

	
	
	while(1){
		instCtrl(0x01);
		displayMenu();
		
		
		while(!RD4){}
		while(RD4){
			DATA = PORTD & 0x0F;
			switch(DATA){
				case 0x00:
					
					displayDispense();
					for (int i = 5; i >= 1; i--){
						instCtrl(0x9D);
						dataCtrl(i + 0x30);
						instCtrl(0x9E);
						dataCtrl('s');
						delay(122);
							if(INTF_flag == 1){
								displayEStop();
								delay(244);
								INTF_flag = 0;
								i = 0;
							}
					}
					break;

				case 0x01:
					displayDispense();
					for (int i = 3; i >= 1; i--){
						instCtrl(0x9D);
						dataCtrl(i + 0x30);
						instCtrl(0x9E);
						dataCtrl('s');
						delay(122);
							if(INTF_flag == 1){
								displayEStop();
								delay(244);
								INTF_flag = 0;
								i = 0;
							}
					}
					break;

				case 0x02:
					displayDispense();
					for (int i = 5; i >= 1; i--){
						instCtrl(0x9D);
						dataCtrl(i + 0x30);
						instCtrl(0x9E);
						dataCtrl('s');
						delay(122);
							if(INTF_flag == 1){
								displayEStop();
								delay(244);
								INTF_flag = 0;
								i = 0;
							}
					}
					break;

				case 0x04:
					displayDispense();
					for (int i = 3; i >= 1; i--){
						instCtrl(0x9D);
						dataCtrl(i + 0x30);
						instCtrl(0x9E);
						dataCtrl('s');
						delay(122);
							if(INTF_flag == 1){
								displayEStop();
								delay(244);
								INTF_flag = 0;
								i = 0;
							}
					}
					break;

				default: break;
			}
		}
		
	}

}