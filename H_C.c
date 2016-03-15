/*
 * H_C_Test.c
 *
 * Created: 3/7/2016 3:59:38 PM
 *  Author: martijus
 */ 


//#Defines:
#define CPU_PRESCALE(n) (CLKPR = 0x80, CLKPR = (n))
#define CPU_16MHz 0x00
#define F_CPU CPU_16MHz 

// Libraries:
#include <avr/io.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "uart.h"
#include "Timer.h"
#include "ADC.h"
#include "Battery.h"

volatile unsigned int count_ms = 0;
volatile unsigned int timerFlag = 0;
void Counter_init (void);

ISR(TIMER1_COMPA_vect){
	count_ms ++;
	timerFlag = 1;
}


// Global Variables:
uint8_t input;

/* moved to SD_PCR_v2.c
volatile uint8_t denTime;
volatile uint8_t denTemp;
volatile uint8_t annTime;
volatile uint8_t annTemp;
volatile uint8_t eloTime;
volatile uint8_t eloTemp;
*/

char passData[19];

// Testing:
#define LED_ON		(PORTD	|= (1<< PD6))	// Toggle Teensy LED on.
#define LED_OFF		(PORTD	&= ~(1<<PD6))	// Toggle Teensy LED off.

// Heating:
#define Heat_ON		(PORTB	|= (1<< PB2))	// Toggle H-bridge channel 1 on.
#define Heat_OFF	(PORTB	&= ~(1<<PB2))	// Toggle H-bridge channel 1 off.

// Fan:
#define Fan_ON		(PORTB	|= (1<< PB3))	// Toggle H-bridge channel 2 on.
#define Fan_OFF		(PORTB	&= ~(1<<PB3))	// Toggle H-bridge channel 2 off.

//Function declarations:
//void init(void);
void port_init(void);
void init(void);
void set_PCR(void);

// Global Variables:
uint8_t input;
char passData[19];

void send_serial (uint16_t);
void send_strB(char *s);
double convertTemp(double);






void h_c_init(void){
	
	DDRB |= ((1<<PB1)|(1<<PB2)|(1<<PB3)) ; // Sets the data direction for Port B PB1 (Relay Control), PB2 is Heating PB3 is fan as an output.
	DDRF |= (0<<PF0) | (0<<PF1); // Sets the data direction for port F, PF0 (ADC in), PF1 (Low Battery Signal) as an input.
	DDRD |= (1<<6); // Sets the data direction for port D , PD6 (Teensy LED) as an output.
}

void holdAtTemp(uint16_t targetTemp, uint16_t targetTime){
	
	
	double readTemp;
	double lowerRange = 0;
	double upperRange = 0;
	double temp = 0;
	double avgTemp = 0.0;		
	int numTemps = 0;
	
	lowerRange = targetTemp;// + 0.1;
	upperRange = targetTemp;// + 0.1;
		
	while(count_ms < 2*targetTime){//count increments at quarter seconds
			
		readTemp = convertADC(adc_read());
		/*
		//send_serial("Denaturing temp is "); //send App
		uart_putchar(denTemp);
		uart_putchar('\n');
		_delay_ms(1000);
		//send_serial("readTemp is "); //send App
		uart_putchar(readTemp);
		uart_putchar('\n');
		_delay_ms(1000);
		*/
	
		_delay_ms(1);// don't forget about the delay in the adc_read function!
			
		if (readTemp < lowerRange){
			OCR0A = (82 - (targetTemp-55)*2);	// normalized to PWM @ 82 at 55C and increase duty cycle by 2 PWM values for each degree
			PORTB = Fan_OFF;					// turn off fan
			PORTD = LED_OFF;					// turn off led
		}
		else{
			OCR0A = 255;		// set heater to 0% duty cycle
			PORTB = Fan_OFF;	// power off the fan
			PORTD = LED_ON;		// turn on the led
		}
		
		if (readTemp > (upperRange + 1)){
			OCR0A = 255;		// heater PWM duty cycle set to 0%
			PORTB = Fan_ON;		// turn on the fan
			PORTD = LED_OFF;	// turn off led
		}
				
		if (timerFlag == 0){	// timer flag is true every 1/4 second
			temp += readTemp;	// sums gathered temperatures
			numTemps++;			// keep track of the number of temperatures gathered
			//timerFlag = 0;		// reset the 1/4 second timer flag
		}
		
		if (timerFlag == 1){
			avgTemp = temp / numTemps;	// average the gathered temperature readings
			temp = 0;					// reset the summed temperatures
			numTemps = 0;				// reset the number of gathered temperatures
			timerFlag = 0;
			
			if (((avgTemp*10)/10 >= targetTemp+2) || ((avgTemp*10)/10 <= targetTemp-2)){ // if current temperature is outside of +/- 1C range
				count_ms = 0; //reset counter if temp is outside of +/- 1C range
			}
		
			//send_serial(avgTemp*10/10, count_ms);	//					
		}

	}			
	
	count_ms = 0; //reset counter		
	
	while(count_ms <= 8){
		PORTD = LED_OFF;
	}

	count_ms = 0; //reset counter
		
	while(count_ms <= 8){
		PORTD = LED_ON;
	}	
	
	PORTD = LED_OFF;	
	
}	


void ValidateData(void){
	char getValue = 'a';		// Receive Variable
	char newLine ='#';
	int valid = 0;			// Control Variable
	passData[18] = '\n';	// Array to pass values.
	int i = 0;			// Iterator for array.
	char istrue ='Y';
	int j;
	
	// For testing:
	printStr("Entered Validate Data Function \n"); //send
	//_delay_ms(5000);
	
	
	while(valid == 0){					// Control Structure for validating data values:
		printStr("Input is \n");	//send
		//_delay_ms(5000);
		while (getValue != newLine)		// Takes in all 18 values send and confirms that the values are correct:
		{
			getValue = uart_getchar();	// Receive 6 values with 3 char per value.
			uart_putchar(getValue);		// Transmit them back to the App.
			passData[i] = getValue;		// Put value into array.
			i++;						// Shift array.
			
		}
		uart_putchar('\n');				// After all the char are send needs a delimiter.
		//_delay_ms(5000);				// Pause for app to check
		
		
		printStr("Array is? \n"); //send
		//_delay_ms(5000);				// Pause for app to check
		for (j=0; j<19; j++)
		{
			uart_putchar(passData[j]);
		}
		uart_putchar('\n');				// After all the char are send needs a delimiter.
		//_delay_ms(5000);				// Pause for app to check
		
		
		getValue = 'Y';
		printStr("Is data correct? \n"); //send
		//_delay_ms(5000);
		//getValue = uart_getchar();		// Receive "Y" for correct, else retransmit.
		if (getValue == istrue)			// Check with app if values were valid.
		{
			valid = 1;					// Exit value for control structure.
		}
	}
}

void set_PCR(int* denTime, int* denTemp, int* annTime, int* annTime, int* annTemp, int* eloTime, int* eloTemp){
	// Control Variable
	int hundreds;
	int tens;
	int ones;
	
	//printStr("In set array is \n"); //send
	//_delay_ms(5000);				// Pause for app to check
	for (int j=0; j<19; j++)
	{
		uart_putchar(passData[j]);
	}
	uart_putchar('\n');				// After all the char are send needs a delimiter.
	//_delay_ms(5000);				// Pause for app to check
	
	// Set Denaturation Temperature:
	hundreds = passData[0]- '0';
	tens = passData[1]- '0';
	ones = passData[2]- '0';
	
	denTemp = hundreds * 100 + tens * 10 + ones;	// Shifts hundreds place value into time variable.
	//printStr("Denaturing temp is "); //send App
	uart_putchar(denTemp);
	//uart_putchar('\n');
	_delay_ms(10);
	
	// Set Denaturation Time:
	hundreds = passData[3]- '0';
	tens = passData[4]- '0';
	ones = passData[5]- '0';
	
	denTime = hundreds * 100 +tens * 10 + ones;	// Shifts hundreds place value into time variable.
	//printStr("Denaturing time is "); //send App
	uart_putchar(denTime);
	//uart_putchar('\n');
	_delay_ms(10);
	
	// Set Annealing Temperature:
	hundreds = passData[6]- '0';
	tens = passData[7]- '0';
	ones = passData[8]- '0';
	
	annTemp = hundreds * 100 +tens * 10 + ones;	// Shifts hundreds place value into time variable.
	//printStr("Annealing temp is "); //send App
	uart_putchar(annTemp);
	//uart_putchar('\n');
	_delay_ms(10);
	
	// Set Annealing Time:
	hundreds = passData[9]- '0';
	tens = passData[10]- '0';
	ones = passData[11]- '0';
	
	annTime = hundreds * 100 +tens * 10 + ones;	// Shifts hundreds place value into time variable.
	//printStr("Annealing time is "); //send App
	uart_putchar(annTime);
	//uart_putchar('\n');
	_delay_ms(10);
	
	// Set Elongation Temperature:
	hundreds = passData[12]- '0';
	tens = passData[13]- '0';
	ones = passData[14]- '0';
	
	eloTemp = hundreds * 100 +tens * 10 + ones;	// Shifts hundreds place value into time variable.
	//printStr("Elongation temp is "); //send App
	uart_putchar(eloTemp);
	//uart_putchar('\n');
	_delay_ms(10);
	
	// Set Elongation Time:
	hundreds = passData[15]- '0';
	tens = passData[16]- '0';
	ones = passData[17]- '0';
	
	eloTime = hundreds * 100 +tens * 10 + ones;	// Shifts hundreds place value into time variable.
	//printStr("Elongation time is "); //send App
	uart_putchar(eloTime);
	//uart_putchar('\n');
	_delay_ms(10);
	
	uart_putchar('~');
}

uint8_t userInput(void){
	char getInput;
	char stopPRC = '*';
	//char handshake = "~";
	getInput = uart_getchar(); // Get input from User.
	
	// Special Handling for User Stop PCR:
	if (getInput == stopPRC){ // App sends a "*" if the user requests Stop PCR
		stopPCR();
		while (1){
			printStr("PCR has been Stopped \n"); //send
			_delay_ms(2000);
			printStr("Please turn off PCR device \n"); //send
			_delay_ms(2000);
		}
	}
	return getInput;
}	