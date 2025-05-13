#include <xc.h>

#pragma config FOSC = XT
#pragma config WDTE = OFF
#pragma config PWRTE = ON
#pragma config BOREN = ON
#pragma config LVP = OFF
#pragma config CPD = OFF
#pragma config WRT = OFF
#pragma config CP = OFF

// Global Variables
bit INTF_flag = 0;
unsigned int period = 0;
unsigned char numDisplay[10]; 
unsigned char keypadNum = ' ';
unsigned int sum = 0;
unsigned long product = 0;
unsigned int numerator = 0;
unsigned int denominator = 0;
unsigned char number[6];
void interrupt ISR()
{
	GIE = 0;
	unsigned char DATA;
	if(INTF){
		
		INTF = 0;
		INTF_flag = 1;
		DATA = (PORTB & 0xF0);
		switch(DATA){
			case 0x00: keypadNum = '1'; break;
			case 0x10: keypadNum = '2'; break;
			case 0x20: keypadNum = '3'; break;
			case 0x40: keypadNum = '4'; break;
			case 0x50: keypadNum = '5'; break;
			case 0x60: keypadNum = '6'; break;
			case 0x80: keypadNum = '7'; break;
			case 0x90: keypadNum = '8'; break;
			case 0xA0: keypadNum = '9'; break;
			case 0xC0: keypadNum = '*'; break;
			case 0xD0: keypadNum = '0'; break;
			case 0xE0: keypadNum = '#'; break;
		}
	}
	GIE = 1;
}

void delayLCD(int MUL) {
    int i, j;
    for (i = MUL; i != 0; i--) {
        for (j = 0; j < 256; j++) {} 
    }
}

//LCD Display Lines
//LINE 1 (80H - 93H)
//LINE 2 (C0H - D3H)
//LINE 3 (94H - A7H)
//LINE 4 (D4H - E7H)
void instCtrl (unsigned char INST)
{
	PORTD = INST; // load instruction to PORTB
	RC0 = 0; // set RS to 0 (instruction reg)
	RC1 = 0; // set RW to 0 (write)
	RC2 = 1; // set E to 1
	delayLCD(1); // 1 ms delay (estimate)
	RC2 = 0; // set E to 0 (strobe)
}

void dataCtrl (unsigned char DATA)
{
	PORTD = DATA; 		// load data to PORTB
	RC0 = 1; 			// set RS to 1 (data reg)
	RC1 = 0; 			// set RW to 0 (write)
	RC2 = 1; 			// set E to 1
	delayLCD(1); 			// 1 ms delay (estimate)
	RC2 = 0; 			// set E to 0 (strobe)
}

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

//void intToASCII(unsigned int num, char *str) {
//    int i = 0;
//    char temp[6]; 
//
//    if (num == 0) {
//        str[i] = '0';
//        i++;
//    } 
//    else {
//        while (num > 0) {
//            temp[i] = (num % 10) + '0'; // Convert digit to ASCII
//            i++;
//            num /= 10;
//    	}
//    }
//
//    // Reverse the number string
//    int j;
//    for (j = 0; j < i; j++) {
//        str[j] = temp[i - j - 1];
//    }
//    str[i] = '\0'; // Null terminator
//}

void intToASCII(unsigned long num, char *str) {
    int i = 5;  // Start filling from the last digit
    str[6] = '\0';  // Null terminator for 6-character string

    // Fill each position with the digit or 0 if num is finished
    while (i >= 0) {
        str[i] = (num % 10) + '0';
        num /= 10;
        i--;
    }

}

void main() {
	
	// USART Configuration Transmission
	SPBRG = 0x19; // 9.6K baud rate @ FOSC=4MHz, asynchronous high speed
	// (see formula in Table 10-1)
	SYNC = 0; // asynchronous mode (TXSTA reg)
	SPEN = 1; // enable serial port (RCSTA reg)
	TX9 = 0; // 8-bit transmission (TXSTA reg)
	BRGH = 1; // asynchronous high-speed (TXSTA reg)
	TXEN = 1; // transmit enable (TXSTA reg) 
	TRISC6 = 0;
	TRISC7 = 1;
	
    // Configure ports
    TRISB = 0xFF;
    TRISC = 0xF8;
    TRISD = 0x00;
	
	// For Timer0
	OPTION_REG = 0xC4;
	
	// For interrupt RB0
	INTEDG = 1; // interrupt at rising edge
 	INTE = 1; // enable RB0/INT external interrupt
 	INTF = 0; // clears the interrupt flag
	
	GIE = 1;
	
	initLCD();						// Initialize LCD
    int count = 0;
    int index = 0;
    instCtrl(0x91);
	while (1) {
    	
        if(INTF_flag){
        	INTF_flag = 0;
        	
        	while(!TXIF);
        	TXREG = keypadNum;
        	
        		
        	
        	//store input in an array
//        	if(keypadNum >= '0' && keypadNum <= '9')
//        	{
//        		dataCtrl(keypadNum);
//        		count++;
//        		number[index++] = keypadNum;
//        		number[index] = '\0';	
//			}
//			
//			if(count == 3){
//        		instCtrl(0xD1);
//        		count = 0;
//        		//while(count == 0);
//			}
//			
//			
//        	
//        	if(keypadNum == '*')		// Addition
//			{
//        		instCtrl(0xA2);
//        		display("------");
//        		numerator = 0;
//				denominator = 0;
//        		for (int i = 0; i < 6; i++) {
//			    	int temp = number[i] - '0';  // Convert char to int
//				    if (i <=2) {
//				        switch (i) {
//				            case 0: temp *= 100; break;  // Hundreds place
//				            case 1: temp *= 10;  break;  // Tens place
//				            case 2: temp *= 1;   break;  // Ones place
//				        }
//				        numerator += temp;
//				    }
//				    else if (i > 2 && i <=5)
//				    {
//				    	switch (i) {
//				            case 3: temp *= 100; break;  // Hundreds place
//				            case 4: temp *= 10;  break;  // Tens place
//				            case 5: temp *= 1;   break;  // Ones place
//				        }
//				        denominator += temp;
//					}
//				}
//				instCtrl(0xE2);
//				sum = numerator + denominator;
//				intToASCII(sum, numDisplay);
//				display(numDisplay);
//				index = 0;
//			}
//			else if(keypadNum == '#')		// Multiplication 
//			{
//				instCtrl(0xA2);
//        		display("------");
//        		
//        		numerator = 0;
//				denominator = 0;
//        		for (int i = 0; i < 6; i++) {
//			    	int temp = number[i] - '0';  // Convert char to int
//				    if (i <=2) {
//				        switch (i) {
//				            case 0: temp *= 100; break;  // Hundreds place
//				            case 1: temp *= 10;  break;  // Tens place
//				            case 2: temp *= 1;   break;  // Ones place
//				        }
//				        numerator += temp;
//				    }
//				    else if (i > 2 && i <=5)
//				    {
//				    	switch (i) {
//				            case 3: temp *= 100; break;  // Hundreds place
//				            case 4: temp *= 10;  break;  // Tens place
//				            case 5: temp *= 1;   break;  // Ones place
//				        }
//				        denominator += temp;
//					}
//				}
//				instCtrl(0xE2);
//				product = (unsigned long)numerator * (unsigned long)denominator;
//				intToASCII(product, numDisplay);
//				display(numDisplay);
//				index = 0;
//			
//			}
        	
		}
    }
}
